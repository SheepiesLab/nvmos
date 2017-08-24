//
// Created by benng on 7/18/17.
//

#include <kernel/mman/Heap.h>
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

    // Construct FreeBlockList
    HeapFreeBlockNode *currentFBN = (HeapFreeBlockNode *) heapStart;
    currentFBN->last = NULL;

    for (size_t i = 0; i < heap->blockCount - 1; ++i) {
        currentFBN->next = (HeapFreeBlockNode *) (currentFBN +
                                                  blockSize);
        currentFBN->next->last = currentFBN;
        currentFBN = currentFBN->next;
    }

    currentFBN->next = NULL;

    return 0;

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

    // Find consecutive free blocks
    HeapFreeBlockNode *currentPtr = heap->heapFreeBlockListHead;
    while (true) {
        bool allocated = true;
        kptr_t allocAddr = currentPtr;
        for (size_t i = 0; i < blocksNeeded - 1; ++i) {
            if (currentPtr->next == NULL) {
                return NULL;
            }
            if (currentPtr->next - currentPtr != heap->blockSize) {
                allocated = false;
                break;
            }
            currentPtr = currentPtr->next;
        }
        if (allocated) {
            HeapAllocationHeader *header = allocAddr;
            header->blocksAllocated = blocksNeeded - 1;
            return allocAddr + heap->blockSize;
        }
        currentPtr = currentPtr->next;
    }

    return NULL;

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
    kptr_t releaseStartAddr = header;

    HeapFreeBlockNode *currentPtr = heap->heapFreeBlockListHead;

    while (true) {
        if (currentPtr > releaseStartAddr) {
            if ((currentPtr - releaseStartAddr) % heap->blockSize !=
                0) {
                return -1;
            }
            if ((currentPtr - releaseStartAddr) / heap->blockSize <
                blocksReleased) {
                return -1;
            }
            HeapFreeBlockNode *currentFBN =
                    (HeapFreeBlockNode *) releaseStartAddr;
            currentFBN->last = NULL;

            for (size_t i = 0; i < blocksReleased; ++i) {
                currentFBN->next = (HeapFreeBlockNode *) (currentFBN +
                                                          blockSize);
                currentFBN->next->last = currentFBN;
                currentFBN = currentFBN->next;
            }

            currentFBN->next = currentPtr;
            heap->heapFreeBlockListHead = releaseStartAddr;
            return 0;
        }

        if (currentPtr->next == NULL) {
            break;
        }
        currentPtr = currentPtr->next;
    }



}