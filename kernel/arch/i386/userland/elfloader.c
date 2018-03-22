#include <kernel/userland/elfloader.h>
#include <kernel/userland/elf.h>
#include <math.h>

#define MAX_FILE_BLKSIZE 0x400

// Implementation assume file is in consecutive blocks
// If it is not the case, I need to build virtual address
// space for kernel first, which I have no plan to do.
//
// I don't want to copy it to kernel memory either, as
// it defeats the purpose of execution in-place
size_t elf_loadExec(
	file_meta_t *exec,
	proc_meta_t *proc,
	nvmos_dl_allocator_t *alloc)
{
	nvmos_ptr_t execBlk;

	size_t res = 0;

	file_getMap(
		exec,
		&execBlk,
		0,
		1);

	Elf32_Ehdr *elfHeader = (Elf32_Ehdr *)execBlk;
	if (!elf_check_supported(elfHeader))
	{
		return 0;
	}

	if (elfHeader->e_type != ET_EXEC)
	{
		ERROR("ELF File cannot be executed.\n");
		return 0;
	}

	Elf32_Phdr **pHdrs =
		(Elf32_Phdr **)((uint32_t)elfHeader + elfHeader->e_phoff);

	int i;
	for (i = 0; i < elfHeader->e_phnum; ++i)
	{
		if (pHdrs[i]->p_type != PT_LOAD)
			continue;

		size_t fileoff = pHdrs[i]->p_offset;
		size_t blkIdx = fileoff / 0x1000;
		size_t len =
			roundup((fileoff % 0x1000) + pHdrs[i]->p_memsz,
					0x1000);
		res += len;

		uint8_t rwx;
		if (pHdrs[i]->p_flags & PF_R)
			rwx |= PROC_R;
		if (pHdrs[i]->p_flags & PF_W)
			rwx |= PROC_W;

		nvmos_ptr_t procAddr = pHdrs[i]->p_vaddr;

		int res = proc_mapFile(
			proc,
			exec,
			blkIdx,
			len,
			rwx,
			procAddr,
			alloc);
	}
	return res;
}

#define PROC_EXEC_ADDR 0x40000000
#define PROC_KSTACK_ADDR 0xffffc000
#define PROC_USTACK_ADDR 0xffffa000

int elf_exec(
	file_meta_t *exec,
	proc_meta_t *proc,
	nvmos_dl_allocator_t *alloc)
{
	size_t res;
	if (res = elf_loadExec(exec, proc, alloc) == 0)
	{
		return -1;
	}

	nvmos_ptr_t kstack = nvmos_dl_alloc_allocateBlocks(
		alloc,
		2);

	if (kstack == NULL)
	{
		ERROR("Kernel stack allocation failed");
		return -1;
	}

	if (proc_mapKernel(proc, PROC_KSTACK_ADDR, kstack, 2, alloc))
	{
		ERROR("Kernel stack mapping failed");
		return -1;
	}

	if (proc_allocBlks(proc, PROC_USTACK_ADDR, 2, alloc))
	{
		ERROR("User stack allocation failed");
		return -1;
	}
}