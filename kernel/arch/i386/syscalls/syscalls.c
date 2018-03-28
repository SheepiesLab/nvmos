#include <kernel/syscalls/syscalls.h>
#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/meta.h>
#include <kernel/datalayer/file.h>
#include <kernel/datalayer/directory.h>
#include <kernel/datalayer/proc.h>
#include <kernel/userland/userland.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint32_t nvmos_syscall(
	uint32_t paramsPtr,
	uint32_t paramCount,
	uint32_t userESP,
	uint32_t userRetAddr)
{
	// printf("SYSCALL!\n");
	// printf("EAX (paramsPtr):   0x%p\n", (uint64_t)paramsPtr);
	// printf("EBX (paramCount):  0x%p\n", (uint64_t)paramCount);
	// printf("ECX (ESP of user): 0x%p\n", (uint64_t)userESP);
	// printf("EDX (return addr): 0x%p\n", (uint64_t)userRetAddr);

	if (paramsPtr == 0xdeadbeef)
	{
		printf("HALT!\n");
		asm("hlt");
	}

	uint32_t *syscall_no = paramsPtr;
	uint32_t paramsBuf[paramCount + 1];
	memcpy(paramsBuf, (void *)paramsPtr, (paramCount + 1) * sizeof(uint32_t));

	switch (*syscall_no)
	{
		//uint32_t prints(void*,size_t);
	case 0x10:
	{
		uint8_t *bufferPtr = (void *)paramsBuf[1];
		uint32_t bufferSize = paramsBuf[2];
		const unsigned char *bytes = (const unsigned char *)bufferPtr;
		for (size_t i = 0; i < bufferSize; i++)
			if (putchar(bytes[i]) == EOF)
				return i;
		return bufferSize;
		break;
	}

	//int open(const char *pathname, int flags);
	case 0x11:
	{
		nvmos_pagingOff();

		uint8_t *pathname = (void *)paramsBuf[1];
		int32_t flags = paramsBuf[2];
		nvmos_dl_datalayerMeta_t *dlmeta =
			*(nvmos_dl_datalayerMeta_t **)(PROC_KSTACK_ADDR);
		proc_meta_t *proc = *(proc_meta_t **)(PROC_KSTACK_ADDR + 4);
		nvmos_dl_allocator_t *allocator =
			*(nvmos_dl_allocator_t **)(PROC_KSTACK_ADDR + 8);
		uint32_t *openedFiles = PROC_KSTACK_ADDR + 12;
		uint32_t *nextMmapAddr = PROC_KSTACK_ADDR + 16;
		proc_fdtable_t *fdtable = PROC_KSTACK_ADDR + 20;

		meta_meta_t *uroot = dlmeta->uroot;
		file_meta_t *urootMeta = &(uroot->metaContent.fileMeta);
		if (dir_fileNameUsed(urootMeta, pathname))
		{
			dir_fileRefId_t fileRefId = dir_searchFileRef(urootMeta, pathname);
			dir_fileRef_t *fileRef = dir_getFileRefById(urootMeta, fileRefId);
			fdtable[*openedFiles].fileMeta =
				&(((meta_meta_t *)(fileRef->metaPtr))->metaContent.fileMeta);
		}
		else
		{
			meta_metaBlk_t **metaBlks = &(dlmeta->metaBlockList);
			meta_meta_t *newFile = meta_getNextFreeMeta(metaBlks, allocator);
			file_meta_t *newFileMeta = &(newFile->metaContent.fileMeta);
			newFileMeta->blkSize = 0;
			newFileMeta->fileSize = 0;
			newFileMeta->refCount = 0;
			newFileMeta->_1stBlk = 0;
			newFileMeta->_1stPtrBlk = 0;
			newFileMeta->_2ndPtrBlk = 0;
			newFileMeta->_3rdPtrBlk = 0;
			dir_addFileRef(urootMeta, pathname, newFile, allocator);
			fdtable[*openedFiles].fileMeta = newFileMeta;
			fdtable[*openedFiles].mmap_addr = 0;
			fdtable[*openedFiles].mmapped_blks = 0;
		}

		nvmos_pagingOn(proc->pageDir);
		return (*openedFiles)++;
		break;
	}
		//int ftruncate(int fd, off_t length);
	case 0x13:
	{

		nvmos_pagingOff();

		int fd = paramsBuf[1];
		size_t flen = paramsBuf[2];
		nvmos_dl_datalayerMeta_t *dlmeta =
			*(nvmos_dl_datalayerMeta_t **)(PROC_KSTACK_ADDR);
		proc_meta_t *proc = *(proc_meta_t **)(PROC_KSTACK_ADDR + 4);
		nvmos_dl_allocator_t *allocator =
			*(nvmos_dl_allocator_t **)(PROC_KSTACK_ADDR + 8);
		uint32_t *openedFiles = PROC_KSTACK_ADDR + 12;
		uint32_t *nextMmapAddr = PROC_KSTACK_ADDR + 16;
		proc_fdtable_t *fdtable = PROC_KSTACK_ADDR + 20;

#define RET(retres)                    \
	{                                  \
		nvmos_pagingOn(proc->pageDir); \
		return retres;                 \
	}

		if (fd >= *openedFiles)
		{
			RET(999)
		}
		file_meta_t *fileMeta = fdtable[fd].fileMeta;
		if (fileMeta == NULL)
		{
			RET(999)
		}
		int off = flen - fileMeta->fileSize;
		if (off > 0)
		{
			unsigned i;
			for (i = 0; i < flen;)
			{
				size_t res = file_append(
					fileMeta,
					"\0\0\0\0\0\0\0\0",
					8,
					allocator);
				if (res == 0)
					RET(999)
				i += res;
			}
			if (i >= flen)
				RET(0)
			else
				RET(999)
		}
		break;
#undef RET
	}
	// mmap
	case 0x14:
	{
		nvmos_pagingOff();

		// void *addr = paramsBuf[1];
		size_t length = paramsBuf[2];
		int fd = paramsBuf[5];
		size_t offset = paramsBuf[6];

		nvmos_dl_datalayerMeta_t *dlmeta =
			*(nvmos_dl_datalayerMeta_t **)(PROC_KSTACK_ADDR);
		proc_meta_t *proc = *(proc_meta_t **)(PROC_KSTACK_ADDR + 4);
		nvmos_dl_allocator_t *allocator =
			*(nvmos_dl_allocator_t **)(PROC_KSTACK_ADDR + 8);
		uint32_t *openedFiles = PROC_KSTACK_ADDR + 12;
		uint32_t *nextMmapAddr = PROC_KSTACK_ADDR + 16;
		proc_fdtable_t *fdtable = PROC_KSTACK_ADDR + 20;

#define RET(retres)                    \
	{                                  \
		nvmos_pagingOn(proc->pageDir); \
		return retres;                 \
	}

		if (fdtable[fd].mmapped_blks != 0)
			RET(0)
		file_meta_t *file = fdtable[fd].fileMeta;
		if (length > file->fileSize)
			length = file->fileSize;
		length = roundup(length, 0x1000);
		length /= 0x1000;
		offset /= 0x1000;
		if (proc_mapFile(
				proc,
				file,
				offset,
				length,
				0,
				*nextMmapAddr,
				allocator))
		{
			RET(0)
		}
		fdtable[fd].mmap_addr = *nextMmapAddr;
		fdtable[fd].mmapped_blks = length;
		*nextMmapAddr += length * 0x1000;
		RET(fdtable[fd].mmap_addr)

#undef RET
	}
	case 0xdeadbeef:
		printf("HALT!\n");
		asm("hlt");
		break;
	default:
		return 999;
	}

	return 0xcdedabcd;
}