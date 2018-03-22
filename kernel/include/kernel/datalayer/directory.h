#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/meta.h>
#include <kernel/datalayer/file.h>
#include <kernel/datalayer/ptrBlks.h>
#include <kernel/datalayer/allocator/allocator.h>

struct fileRef
{
	uint8_t fileName[252];
	uint32_t metaPtr;
};
typedef struct fileRef dir_fileRef_t;

struct fileRefBlock
{
	dir_fileRef_t fileRefs[16];
};
typedef struct fileRefBlock dir_fileRefBlk_t;

typedef uint32_t dir_fileRefId_t;
#define dir_fileRefId_inval 0xFFFFFFFF

bool dir_isDir(meta_meta_t *dir);

dir_fileRefId_t dir_addFileRef(
	file_meta_t *dir,
	uint8_t *fileName,
	meta_meta_t *fileMeta,
	nvmos_dl_allocator_t *allocator);

dir_fileRefId_t dir_rePosFileRef(
	file_meta_t *dir,
	dir_fileRefId_t fileRefId);

dir_fileRefId_t dir_searchFileRef(
	file_meta_t *dir,
	char *fileName);

dir_fileRef_t *dir_getFileRefById(
	file_meta_t *dir,
	dir_fileRefId_t id);

bool dir_fileNameUsed(
	file_meta_t *dir,
	char *fileName);

dir_fileRefId_t dir_renameFileRef(
	file_meta_t *dir,
	char *fileName,
	char *newFileName);

int dir_delFileRef(
	file_meta_t *dir,
	char *fileName,
	nvmos_dl_allocator_t *allocator);

#endif