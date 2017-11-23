//
// Created by benng on 7/18/17.
//

#ifndef NVMOS_HEAP_H
#define NVMOS_HEAP_H

#include <kernel/kdef.h>
#include <stddef.h>

typedef struct heapFreeBlockNode HeapFreeBlockNode;

typedef struct heap {
    HeapFreeBlockNode *heapFreeBlockListHead;
    nvmos_ptr_t heapStart;
    nvmos_ptr_t heapEnd;
    nvmos_size_t blockSize;
    nvmos_size_t blockCount;
} Heap;

nvmos_size_t heap_blockSize();

int heap_construct(
        Heap *heap,
        nvmos_ptr_t heapStart,
        nvmos_ptr_t heapEnd);

nvmos_ptr_t heap_malloc(Heap *heap, nvmos_size_t size);

nvmos_ptr_t heap_calloc(Heap *heap, nvmos_size_t len, nvmos_size_t size);

int heap_free(Heap *heap, nvmos_ptr_t loc);

#endif //NVMOS_HEAP_H
