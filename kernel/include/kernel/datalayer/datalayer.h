#ifndef _NVMOS_DATALAYER_H_
#define _NVMOS_DATALAYER_H_

#include <stdint.h>
#include <kernel/kdef.h>
#include <kernel/datalayer/meta.h>
#include <kernel/datalayer/allocator/allocator.h>

#define BLOCK_SIZE 0x1000

// #define NVMOS_DL_DLMETA_FLAG_LOCKED 0x1

#define NVMOS_DL_MAGIC 0x4567858546467575

typedef struct
{
	uint64_t magic;
	uint64_t startAddress;
	uint64_t blockCount;
	uint64_t allocationBlockSize;
	uint64_t kroot;
	uint64_t uroot;
	uint64_t freeBlockList;
	uint64_t metaBlockList;
	uint32_t flags;
} nvmos_dl_datalayerMeta_t;

nvmos_dl_datalayerMeta_t *datalayer_loadDatalayer(nvmos_ptr_t start);

nvmos_dl_datalayerMeta_t *datalayer_createDatalayer(
	nvmos_ptr_t start,
	size_t size,
	size_t allocationBlockSize,
	nvmos_dl_allocator_t *allocator);

size_t datalayer_getSizeOfDatalayer(
	nvmos_dl_datalayerMeta_t *dlmeta);

size_t datalayer_getPageSizeOfDatalayer(
	nvmos_dl_datalayerMeta_t *dlmeta);

int datalayer_resizeDatalayer(
	nvmos_dl_datalayerMeta_t *dlmeta,
	size_t newSize);

int datalayer_getAllocator(
	nvmos_dl_allocator_t *allocator,
	nvmos_dl_datalayerMeta_t *dlmeta);

meta_meta_t *datalayer_getURoot(
	nvmos_dl_datalayerMeta_t *dlmeta);

meta_meta_t *datalayer_getKRoot(
	nvmos_dl_datalayerMeta_t *dlmeta);

#endif