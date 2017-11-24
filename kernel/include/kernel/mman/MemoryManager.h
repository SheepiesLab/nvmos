//
// Created by benng on 7/17/17.
//

#ifndef NVMOS_MEMORYMANAGER_H
#define NVMOS_MEMORYMANAGER_H

#include <kernel/multiboot.h>
#include <kernel/mman/MemoryMap.h>
#include <kernel/mman/heap/Heap.h>
#include <stddef.h>

#define GDT_LEN 4

typedef struct mman {
    multiboot_info_t *mbt;
    uint8_t gdtBuffer[GDT_LEN * 8];
    Heap heap;
} MemoryManager;

int mman_construct(MemoryManager *mman, multiboot_info_t *mbt);

int mman_getMemoryMap(MemoryManager *mman, MemoryMap *mmap, size_t max);

size_t mman_getMemoryMapLength(MemoryManager *mman);

#endif //NVMOS_MEMORYMANAGER_H
