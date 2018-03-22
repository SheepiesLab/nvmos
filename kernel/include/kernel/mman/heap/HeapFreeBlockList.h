//
// Created by benng on 8/25/17.
//

#ifndef NVMOS_HEAPFREEBLOCKLIST_H
#define NVMOS_HEAPFREEBLOCKLIST_H

struct heapFreeBlockNode {
    HeapFreeBlockNode *last;
    HeapFreeBlockNode *next;
};

int heapfbll_construct(Heap *heap);

HeapFreeBlockNode *heapfbll_prev(HeapFreeBlockNode *this);

HeapFreeBlockNode *heapfbll_next(HeapFreeBlockNode *this);

HeapFreeBlockNode *heapfbll_first(HeapFreeBlockNode *this);

HeapFreeBlockNode *heapfbll_last(HeapFreeBlockNode *this);

int heapfbll_insert(Heap *heap, nvmos_ptr_t start, size_t blocks);

nvmos_ptr_t heapfbll_pop(Heap *heap, size_t blocks);

#endif //NVMOS_HEAPFREEBLOCKLIST_H
