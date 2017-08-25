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

int heap_construct(
        Heap *heap,
        kptr_t heapStart,
        kptr_t heapEnd,
        size_t blockSize) {

    if (heap == NULL) {
        return -1;
    }

    // Block cannot hold free block list node
    if (
            blockSize < sizeof(HeapFreeBlockNode) ||
            blockSize < sizeof(HeapAllocationHeader)) {
        return -1;
    }

    // Fill metadata
    heap->heapFreeBlockListHead = (HeapFreeBlockNode *) heapStart;
    heap->heapStart = heapStart;
    heap->heapEnd = heapEnd;
    heap->blockSize = blockSize;
    heap->blockCount = (heapEnd - heapStart) / blockSize;

    return heapfbll_construct(heap);

}

kptr_t heap_malloc(Heap *heap, size_t size) {

    if (heap == NULL) {
        return NULL;
    }

    size_t blocksNeeded =
            (size + sizeof(HeapAllocationHeader)) / heap->blockSize;
    if ((size + sizeof(HeapAllocationHeader)) % heap->blockSize != 0)
        ++blocksNeeded;
    ++blocksNeeded; // For allocation header

    kptr_t allocAddr = heapfbll_pop(heap,blocksNeeded);
    if(allocAddr == NULL) return NULL;

    HeapAllocationHeader *header = (HeapAllocationHeader *)allocAddr;
    header->blocksAllocated = blocksNeeded - 1;

    return allocAddr + heap->blockSize;

}

kptr_t heap_calloc(Heap *heap, size_t len, size_t size) {

    return heap_malloc(heap, len * size);

}

int heap_free(Heap *heap, kptr_t loc) {

    if (heap == NULL) {
        return -1;
    }

    if (loc == NULL) {
        return -1;
    }

    HeapAllocationHeader *header = loc - heap->blockSize;
    size_t blocksReleased = header->blocksAllocated + 1;
    kptr_t releaseStartAddr = (kptr_t)header;

    return heapfbll_insert(heap, releaseStartAddr, blocksReleased);

}