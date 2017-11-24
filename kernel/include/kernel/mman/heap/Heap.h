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
    nvmos_pointer_t heapStart;
    nvmos_pointer_t heapEnd;
    size_t blockSize;
    size_t blockCount;
} Heap;

size_t heap_blockSize();

int heap_construct(
        Heap *heap,
        nvmos_pointer_t heapStart,
        nvmos_pointer_t heapEnd);

nvmos_pointer_t heap_malloc(Heap *heap, size_t size);

nvmos_pointer_t heap_calloc(Heap *heap, size_t len, size_t size);

int heap_free(Heap *heap, nvmos_pointer_t loc);

#endif //NVMOS_HEAP_H
