#ifndef NVMOS_DL_ALLOCATOR_H
#define NVMOS_DL_ALLOCATOR_H

#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/allocator/redBlackTree/rbt.h>
#include <stdint.h>

typedef struct
{
	nvmos_pointer_t sameValueNext;
	nvmos_pointer_t segmentHead;
	nvmos_pointer_t segmentTail;
	rbt_node_t redBlackTreeNode;
} nvmos_dl_freeBlockNode_t;

typedef struct
{
	size_t allocationBlockSize;
	rbt_node_t *head;
} nvmos_dl_allocator_t;

int nvmos_dl_alloc_createAllocator(
	nvmos_dl_allocator_t *allocator,
	nvmos_pointer_t startAddress,
	size_t length,
	size_t allocationBlockSize);

int nvmos_dl_alloc_retrieveAllocator(
	nvmos_dl_allocator_t *allocator,
	nvmos_pointer_t head,
	size_t allocationBlockSize);

nvmos_pointer_t nvmos_dl_alloc_allocateBlocks(
	nvmos_dl_allocator_t *allocator,
	size_t blockCount);

int nvmos_dl_alloc_deallocateBlocks(
	nvmos_dl_allocator_t *allocator,
	nvmos_pointer_t startBlock,
	size_t length);

#endif
