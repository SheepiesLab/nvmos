#ifndef NVMOS_DL_ALLOC_H
#define NVMOS_DL_ALLOC_H

#include <kernel/datalayer/dataStruceture.h>

#define NVMOS_DL_ALLOC_INTREE_NODE 0x1
#define NVMOS_DL_ALLOC_RED_NODE 0x2
#define NVMOS_DL_ALLOC_BLACK_NODE 0x0
#define NVMOS_DL_ALLOC_SEGMENT_HEAD_NODE 0x4
#define NVMOS_DL_ALLOC_SEGMENT_TAIL_NODE 0x0

typedef struct {
	uint32_t	length;
	uint32_t	flags;
	uint64_t	sameValueNext;
	uint64_t	parent;
	uint64_t	leftChild;
	uint64_t	rightChild;
} nvmos_dl_freeBlockListNode_t;

typedef struct {
	size_t					allocationBlockSize;
	nvmos_dl_freeBlockListNode_t *	head;
} nvmos_dl_allocator_t;

int nvmos_dl_alloc_createAllocator(nvmos_dl_allocator_t *allocator, nvmos_pointer_t startAddress, size_t length, size_t allocationBlockSize);

int nvmos_dl_alloc_retrieveAllocator(nvmos_dl_allocator_t *allocator, nvmos_pointer_t head, size_t allocationBlockSize);

nvmos_pointer_t nvmos_dl_alloc_allocateBlocks(nvmos_dl_allocator_t *allocator, size_t blockCount);

int nvmos_dl_alloc_deallocateBlocks(nvmos_dl_allocator_t *allocator, nvmos_pointer_t startBlock, size_t length);

#endif
