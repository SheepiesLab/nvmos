#ifndef NVMOS_DL_META_H_
#define NVMOS_DL_META_H_

#define META_FLAG_ISFILE 0x1 // Either file meta or process meta
#define META_FLAG_ISDIR 0x2
#define META_FLAG_FREE 0x4
#define META_FLAG_LOCKED 0x8

#include <stdint.h>
#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/file.h>
#include <kernel/datalayer/proc.h>
#include <kernel/datalayer/allocator/allocator.h>



struct meta_freeMeta
{
	uint64_t nextFreeMeta;
};
typedef struct meta_freeMeta meta_freeMeta_t;

struct meta_meta
{
	uint64_t metaId;
	union {
		file_meta_t fileMeta;
		proc_meta_t processMeta;
		meta_freeMeta_t freeMeta;
	} metaContent;
	uint32_t flags;
};
typedef struct meta_meta meta_meta_t;

typedef struct {
	meta_meta_t metas[93];
	uint32_t nextBlk;
} meta_metaBlk_t;

meta_meta_t *meta_getNextFreeMeta(
	meta_metaBlk_t *metaBlk, 
	nvmos_dl_allocator_t *alloc);

bool meta_isFile(meta_meta_t *meta);
bool meta_isDir(meta_meta_t *meta);
bool meta_isProc(meta_meta_t *meta);
bool meta_isFree(meta_meta_t *meta);

void meta_setFile(meta_meta_t *meta);
void meta_setDir(meta_meta_t *meta);
void meta_setProc(meta_meta_t *meta);
void meta_setFree(meta_meta_t *meta);

#endif