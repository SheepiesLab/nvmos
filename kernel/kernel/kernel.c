#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/io/tty.h>
#include <kernel/multiboot.h>
#include <kernel/file.h>
#include <kernel/stdout.h>
#include <kernel/stderr.h>
#include <kernel/mman/MemoryManager.h>
#include <kernel/mman/KernelSection.h>
#include <kernel/mman/HeapManager.h>

void kernel_main(multiboot_info_t *mbt) {
    bool PRINT_SECTION_ADDR = true;
    bool PRINT_MMAP = true;

    uint8_t idtBuffer[256 * 8];

    FILE _stdout;
    FILE _stderr;
    stdout_init(&_stdout);
    stderr_init(&_stderr);
    terminal_initialize();
    printf("Hello, kernel World!\n\n");

    MemoryManager mman;
    mman_construct(&mman, mbt);

    KernelSection *ksects = ksection_getKsections();

    if (PRINT_MMAP) {
        printf("MMap Addr: %p\n", mman.mbt->mmap_addr);

        size_t mmapLength = mman_getMemoryMapLength(&mman);
        printf("MMap Length: %d\n", (int) mmapLength);

        MemoryMap mmap[mmapLength];
        mman_getMemoryMap(&mman, mmap, mmapLength);
        for (int i = 0; i < mmapLength; ++i) {
            printf("Addr: %p; ", mmap[i].addr);
            printf("Len: %p; ", mmap[i].len);
            printf("Type: %d\n", mmap[i].type);
        }
        printf("\n");

        if (PRINT_SECTION_ADDR) {
            printf("Text start:     0x%p\t",
                   ksects[KSECTION_SECTION_TEXT].addr);
            printf("Text length:    0x%p\n",
                   ksects[KSECTION_SECTION_TEXT].len);
            printf("RODATA start:   0x%p\t",
                   ksects[KSECTION_SECTION_RODATA].addr);
            printf("RODATA length:  0x%p\n",
                   ksects[KSECTION_SECTION_RODATA].len);
            printf("DATA start:     0x%p\t",
                   ksects[KSECTION_SECTION_DATA].addr);
            printf("DATA length:    0x%p\n",
                   ksects[KSECTION_SECTION_DATA].len);
            printf("TSS start:      0x%p\t",
                   ksects[KSECTION_SECTION_TSS].addr);
            printf("TSS length:     0x%p\n",
                   ksects[KSECTION_SECTION_TSS].len);
            printf("BSS start:      0x%p\t",
                   ksects[KSECTION_SECTION_BSS].addr);
            printf("BSS length:     0x%p\n",
                   ksects[KSECTION_SECTION_BSS].len);
            printf("HEAP start:     0x%p\t",
                   ksects[KSECTION_SECTION_HEAP].addr);
            printf("HEAP length:    0x%p\n",
                   ksects[KSECTION_SECTION_HEAP].len);
        }
    }


    int *testIntPtr = heapman_malloc(&(mman.heapManager),sizeof(int));
    printf("heapManStart: 0x%p\n",mman.heapManager.heapStart);
    printf("heapManEnd: 0x%p\n",mman.heapManager.heapEnd);
    printf("testPtr: 0x%p\n",testIntPtr);
}
