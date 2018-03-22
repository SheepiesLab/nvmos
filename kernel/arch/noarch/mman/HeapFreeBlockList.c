//
// Created by benng on 8/24/17.
//

#include <kernel/mman/heap/Heap.h>
#include <kernel/mman/heap/HeapFreeBlockList.h>
#include <stdbool.h>

typedef HeapFreeBlockNode HeapFBN;

int heapfbll_construct(Heap *heap) {
    size_t blockSize = heap_blockSize();


    heap->heapFreeBlockListHead = (HeapFBN *) heap->heapStart;

    HeapFreeBlockNode *currentFBN = (HeapFBN *) heap->heapStart;
    currentFBN->last = NULL;

    for (size_t i = 0; i < heap->blockCount - 1; ++i) {
        currentFBN->next = (nvmos_ptr_t) currentFBN + blockSize;
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

int heapfbll_insert(Heap *heap, nvmos_ptr_t start, size_t blocks) {

    Heap dummyHeap;
    dummyHeap.heapFreeBlockListHead = start;
    dummyHeap.heapStart = start;
    dummyHeap.heapEnd = start + heap->blockSize * blocks;
    dummyHeap.blockSize = heap->blockSize;
    dummyHeap.blockCount = blocks;
    heapfbll_construct(&dummyHeap);

    // See if insert before head
    if ((nvmos_ptr_t) (heap->heapFreeBlockListHead) > start) {
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

        if ((nvmos_ptr_t) currentfbn > start) {

        }
    }

    // Insert at the end
    HeapFBN *newListLast = heapfbll_last(
            dummyHeap.heapFreeBlockListHead);
    dummyHeap.heapFreeBlockListHead->last = currentfbn;
    currentfbn->next = dummyHeap.heapFreeBlockListHead;

    return 0;

}

nvmos_ptr_t heapfbll_pop(Heap *heap, size_t blocks) {

    // Find consecutive free blocks
    HeapFBN *currentPtr = (nvmos_ptr_t) heap->heapFreeBlockListHead;
    while (true) {
        bool allocated = true;
        nvmos_ptr_t allocAddr = (nvmos_ptr_t) currentPtr;
        for (int i = 0; i < blocks - 1; ++i) {
            if (currentPtr->next == NULL) {
                return NULL;
            }
            if ((nvmos_ptr_t) currentPtr->next - (nvmos_ptr_t) currentPtr !=
                heap_blockSize()) {
                allocated = false;
                break;
            }
            currentPtr = currentPtr->next;
        }
        if (allocated) {
            HeapFBN *headOfSegment = (HeapFBN *) allocAddr;
            HeapFBN *tailOfSegment = currentPtr;
            if (headOfSegment->last != NULL)
                headOfSegment->last->next = tailOfSegment->next;
            else
                heap->heapFreeBlockListHead = tailOfSegment->next;
            if (heap->heapFreeBlockListHead != NULL)
                heap->heapFreeBlockListHead->last = NULL;
            if (tailOfSegment->next != NULL)
                tailOfSegment->next->last = headOfSegment->last;
            return allocAddr;
        }
        currentPtr = currentPtr->next;
    }

    return NULL;
}