//
// Created by benng on 7/18/17.
//

#ifndef NVMOS_HEAPMANAGER_H
#define NVMOS_HEAPMANAGER_H

#include <kernel/kdef.h>
#include <stddef.h>

typedef struct heapAllocListNode HeapAllocationListNode;

typedef struct heapman HeapManager;

int heapman_construct(HeapManager *heapman, kptr_t heapStart, kptr_t heapEnd);

kptr_t heapman_malloc(HeapManager *heapman, size_t size);

kptr_t heapman_calloc(HeapManager *heapman, size_t len, size_t size);

int heapman_free(HeapManager *heapman, kptr_t loc);

#endif //NVMOS_HEAPMANAGER_H
