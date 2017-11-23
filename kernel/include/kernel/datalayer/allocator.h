#ifndef DL_ALLOCATOR_H
#define DL_ALLOCATOR_H

#include <kernel/datalayer/dataStruceture.h>

#define DL_ALLOC_INTREE_NODE 0x1
#define DL_ALLOC_RED_NODE 0x2
#define DL_ALLOC_BLACK_NODE 0x0
#define DL_ALLOC_SEGMENT_HEAD_NODE 0x4
#define DL_ALLOC_SEGMENT_TAIL_NODE 0x0

typedef struct {
	size_t	length;
	uint8_t flags;
	kptr_t	sameValueNext;
	kptr_t	parent;
	kptr_t	leftChild;
	kptr_t	rightChild;
} datalayer_freeBlockListNode_t;

typedef struct {
	size_t							allocationBlockSize;
	datalayer_freeBlockListNode_t * head;
} datalayer_allocator_t;

int DL_allocator_createAllocator(datalayer_allocator_t *allocator, kptr_t startAddress, size_t length, size_t allocationBlockSize);

int DL_allocator_retrieveAllocator(datalayer_allocator_t *allocator, kptr_t head, size_t allocationBlockSize);

kptr_t DL_allocator_allocateBlocks(datalayer_allocator_t *allocator, size_t blockCount);

int DL_allocator_deallocateBlocks(datalayer_allocator_t *allocator, kptr_t startBlock, size_t length);

#endif
