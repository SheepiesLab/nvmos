//
// Created by benng on 7/17/17.
//

#ifndef NVMOS_MEMORYMAP_H
#define NVMOS_MEMORYMAP_H

#include <stdint.h>

typedef struct mmap {
    uint64_t addr;
    uint64_t len;
#define MMAP_MEMORY_AVAILABLE               1
#define MMAP_MEMORY_RESERVED                2
#define MMAP_MEMORY_ACPI_RECLAIMABLE        3
#define MMAP_MEMORY_NVS                     4
#define MMAP_MEMORY_BADRAM                  5
    uint32_t type;
} MemoryMap;

#endif //NVMOS_MEMORYMAP_H
