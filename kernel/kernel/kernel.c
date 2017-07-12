#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/sectionAddresses.h>
#include <kernel/gdt.h>
#include <kernel/file.h>
#include <kernel/stdout.h>
#include <kernel/stderr.h>

void kernel_main(multiboot_info_t *mbt) {
    bool PRINT_SECTION_ADDR = true;
    bool PRINT_MMAP = true;

    size_t GdtSize = 7;
    uint8_t GDT[GdtSize * 8];

    FILE _stdout;
    FILE _stderr;
    stdout_init(&_stdout);
    stderr_init(&_stderr);
    terminal_initialize();
    printf("Hello, kernel World!\n\n");

    if (PRINT_MMAP) {
        printf("Lower Memory: From 0x0, size %d kb\n", mbt->mem_lower);
        printf("Upper Memory: From 0x100000, size %d kb\n\n", mbt->mem_upper);

        if ((mbt->flags & MULTIBOOT_INFO_MEM_MAP) == MULTIBOOT_INFO_MEM_MAP) {
            printf("MMap Buffer Length: %d\n", mbt->mmap_length);

            multiboot_memory_map_t *mmap = mbt->mmap_addr;
            while (mmap < mbt->mmap_addr + mbt->mmap_length) {
                printf("Entrysize: %d; ", mmap->size);
                printf("Addr: %p; ", mmap->addr);
                printf("Len: %p; ", mmap->len);
                printf("Type: %d\n", mmap->type);

                mmap = (multiboot_memory_map_t *) ((unsigned int) mmap + mmap->size + sizeof(mmap->size));
            }
            printf("\n");
        }
    }

    uint64_t sectionTextStartAddr = getSectionAddress(SECTION_TEXT, SECTION_START);
    uint64_t sectionTextEndAddr = getSectionAddress(SECTION_TEXT, SECTION_END);
    uint64_t sectionRodataStartAddr = getSectionAddress(SECTION_RODATA, SECTION_START);
    uint64_t sectionRodataEndAddr = getSectionAddress(SECTION_RODATA, SECTION_END);
    uint64_t sectionDataStartAddr = getSectionAddress(SECTION_DATA, SECTION_START);
    uint64_t sectionDataEndAddr = getSectionAddress(SECTION_DATA, SECTION_END);
    uint64_t sectionTssStartAddr = getSectionAddress(SECTION_TSS, SECTION_START);
    uint64_t sectionTssEndAddr = getSectionAddress(SECTION_TSS, SECTION_END);
    uint64_t sectionBssStartAddr = getSectionAddress(SECTION_BSS, SECTION_START);
    uint64_t sectionBssEndAddr = getSectionAddress(SECTION_BSS, SECTION_END);
    uint64_t sectionHeapStartAddr = getSectionAddress(SECTION_HEAP, SECTION_START);
    uint64_t sectionHeapEndAddr = getSectionAddress(SECTION_HEAP, SECTION_END);

    if (PRINT_SECTION_ADDR) {
        printf("Text start:     0x%p\t", sectionTextStartAddr);
        printf("Text end:       0x%p\n", sectionTextEndAddr);
        printf("RODATA start:   0x%p\t", sectionRodataStartAddr);
        printf("RODATA end:     0x%p\n", sectionRodataEndAddr);
        printf("DATA start:     0x%p\t", sectionDataStartAddr);
        printf("DATA end:       0x%p\n", sectionDataEndAddr);
        printf("TSS start:      0x%p\t", sectionTssStartAddr);
        printf("TSS end:        0x%p\n", sectionTssEndAddr);
        printf("BSS start:      0x%p\t", sectionBssStartAddr);
        printf("BSS end:        0x%p\n", sectionBssEndAddr);
        printf("HEAP start:     0x%p\t", sectionHeapStartAddr);
        printf("HEAP end:       0x%p\n", sectionHeapEndAddr);
    }

    GlobalDescriptor globalDescriptor[GdtSize];
    globalDescriptor[0].base = 0;
    globalDescriptor[0].limit = 0;
    globalDescriptor[0].type = 0;
    globalDescriptor[1].base = sectionTextStartAddr;
    globalDescriptor[1].limit = sectionTextEndAddr - sectionTextStartAddr;
    globalDescriptor[1].type = 0x9A;
    globalDescriptor[2].base = sectionRodataStartAddr;
    globalDescriptor[2].limit = sectionRodataEndAddr - sectionRodataStartAddr;
    globalDescriptor[2].type = 0x90;
    globalDescriptor[3].base = sectionDataStartAddr;
    globalDescriptor[3].limit = sectionDataEndAddr - sectionDataStartAddr;
    globalDescriptor[3].type = 0x92;
    globalDescriptor[4].base = sectionTssStartAddr;
    globalDescriptor[4].limit = sectionTssEndAddr - sectionTssStartAddr;
    globalDescriptor[4].type = 0x89;
    globalDescriptor[5].base = sectionBssStartAddr;
    globalDescriptor[5].limit = sectionBssEndAddr - sectionBssStartAddr;
    globalDescriptor[5].type = 0x92;
    globalDescriptor[6].base = sectionBssEndAddr;
    globalDescriptor[6].limit = mbt->mem_upper - sectionBssEndAddr;
    globalDescriptor[6].type = 0x92;

    commitGDT(GDT, globalDescriptor, GdtSize);

}
