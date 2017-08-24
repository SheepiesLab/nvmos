//
// Created by benng on 8/24/17.
//

#include <kernel/mman/Heap.h>
#include <stdbool.h>

typedef HeapFreeBlockNode HeapFBN;

struct heapFreeBlockNode {
    HeapFreeBlockNode *last;
    HeapFreeBlockNode *next;
};

int heapfbll_construct(Heap *heap) {

    heap->heapFreeBlockListHead = (HeapFreeBlockNode *) heapStart;

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

HeapFreeBlockNode *heapfbll_prev(HeapFreeBlockNode *this) {
    return this->last;
}

HeapFreeBlockNode *heapfbll_next(HeapFreeBlockNode *this) {
    return this->next;
}

HeapFreeBlockNode *heapfbll_first(HeapFreeBlockNode *this) {
    while (this->last != NULL) {
        this = this->last;
    }
    return this;
}

HeapFreeBlockNode *heapfbll_last(HeapFreeBlockNode *this) {
    while (this->next != NULL) {
        this = this->next;
    }
    return this;
}

int heapfbll_insert(Heap *heap, kptr_t start, size_t blocks) {

    Heap dummyHeap;
    dummyHeap.heapFreeBlockListHead = start;
    dummyHeap.heapStart = start;
    dummyHeap.heapEnd = start + heap->blockSize * blocks;
    dummyHeap.blockSize = heap->blockSize;
    dummyHeap.blockCount = blocks;
    heapfbll_construct(dummyHeap);

    // See if insert before head
    if ((kptr_t) (heap->heapFreeBlockListHead) > start) {
        HeapFBN *newListLast = heapfbll_last(
                dummyHeap.heapFreeBlockListHead);
        newListLast->next = heap->heapFreeBlockListHead;
        heap->heapFreeBlockListHead->last = newListLast;
        heap->heapFreeBlockListHead = dummyHeap.heapFreeBlockListHead;
        return 0;
    }

    // See if insert in middle of list
    HeapFBN *currentfbn = heap->heapFreeBlockListHead;

    while (currentfbn->next != NULL) {
        currentfbn = currentfbn->next;

        if ((kptr_t) currentfbn > start) {

        }
    }

    // Insert at the end
    HeapFBN *newListLast = heapfbll_last(
            dummyHeap.heapFreeBlockListHead);
    dummyHeap.heapFreeBlockListHead->last = currentfbn;
    currentfbn->next = dummyHeap.heapFreeBlockListHead;

    return 0;

}

kptr_t heapfbll_pop(Heap *heap, size_t blocks) {

    // Find consecutive free blocks
    HeapFreeBlockNode *currentPtr = heap->heapFreeBlockListHead;
    while (true) {
        bool allocated = true;
        kptr_t allocAddr = currentPtr;
        for (size_t i = 0; i < blocks - 1; ++i) {
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
            //TODO cut linked list
            return allocAddr;
        }
        currentPtr = currentPtr->next;
    }

    return NULL;
}