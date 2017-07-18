//
// Created by benng on 7/17/17.
//

#ifndef NVMOS_KERNELSECTION_H
#define NVMOS_KERNELSECTION_H

#include <stdint.h>

#define KSECTION_COUNT   5

#define KSECTION_SECTION_TEXT    1
#define KSECTION_SECTION_RODATA  2
#define KSECTION_SECTION_DATA    3
#define KSECTION_SECTION_TSS     4
#define KSECTION_SECTION_BSS     5

typedef struct ksection {
    uint8_t section;
    uint64_t addr;
    uint64_t len;
}KernelSection;

KernelSection *ksection_getKsections();

#endif //NVMOS_KERNELSECTION_H
