//
// Created by benng on 7/18/17.
//

#include <kernel/mman/heap/Heap.h>
#include <kernel/mman/heap/HeapFreeBlockList.h>
#include <string.h>
#include <stdbool.h>

/**
 * Heap Management
 *
 * Heap is managed by a linked list.
 * Linked list node is located in the heap, right after the allocated memory.
 * Allocation list nodes' address includes the node space itself.
 *
 * endAddr - startAddr = sizeOfContent + sizeOfNode
 *
 * Memory is allocated with first-fit policy.
 * Heap cannot overflow as it would not allocate when there is no space.
 */



typedef struct heapAllocationHeader HeapAllocationHeader;
struct heapAllocationHeader {
	size_t blocksAllocated;
};

size_t heap_blockSize()
{
	size_t blockSize;

	if (sizeof(HeapFreeBlockNode) < sizeof(HeapAllocationHeader))
		blockSize = sizeof(HeapAllocationHeader);
	else
		blockSize = sizeof(HeapFreeBlockNode);
	return blockSize;
}

int heap_construct(
	Heap *			heap,
	nvmos_ptr_t heapStart,
	nvmos_ptr_t heapEnd)
{
	if (heap == NULL)
		return -1;

	// Block cannot hold free block list node
	size_t blockSize = heap_blockSize();

	// Fill metadata
	heap->heapFreeBlockListHead = (HeapFreeBlockNode *)heapStart;
	heap->heapStart = heapStart;
	heap->heapEnd = heapEnd;
	heap->blockSize = blockSize;
	heap->blockCount = (heapEnd - heapStart) / blockSize;

	return heapfbll_construct(heap);
}

nvmos_ptr_t heap_malloc(Heap *heap, size_t size)
{
	if (heap == NULL)
		return NULL;

	size_t blocksNeeded =
		(size + sizeof(HeapAllocationHeader)) / heap->blockSize;
	if ((size + sizeof(HeapAllocationHeader)) % heap->blockSize != 0)
		++blocksNeeded;
	++blocksNeeded; // For allocation header

	nvmos_ptr_t allocAddr = heapfbll_pop(heap, blocksNeeded);
	if (allocAddr == NULL) return NULL;

	HeapAllocationHeader *header = (HeapAllocationHeader *)allocAddr;
	header->blocksAllocated = blocksNeeded - 1;

	return allocAddr + heap->blockSize;
}

nvmos_ptr_t heap_calloc(Heap *heap, size_t len, size_t size)
{
	return heap_malloc(heap, len * size);
}

int heap_free(Heap *heap, nvmos_ptr_t loc)
{
	if (heap == NULL)
		return -1;

	if (loc == NULL)
		return -1;

	HeapAllocationHeader *header =
		(HeapAllocationHeader *)(loc - heap->blockSize);
	size_t blocksReleased = header->blocksAllocated + 1;
	nvmos_ptr_t releaseStartAddr = (nvmos_ptr_t)header;

	return heapfbll_insert(heap, releaseStartAddr, blocksReleased);
}
