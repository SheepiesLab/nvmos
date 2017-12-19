#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/io/tty.h>
#include <kernel/io/serial.h>
#include <kernel/int/idt.h>
#include <kernel/int/pic.h>
#include <kernel/int/isr.h>
#include <kernel/multiboot.h>
#include <kernel/file.h>
#include <kernel/stdout.h>
#include <kernel/stderr.h>
#include <kernel/mman/MemoryManager.h>
#include <kernel/mman/KernelSection.h>
#include <kernel/mman/heap/Heap.h>
#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/meta.h>

void kernel_main(multiboot_info_t *mbt)
{

    bool PRINT_DEBUG = true;

    uint8_t idtBuffer[256 * 8];

    FILE _stdout;
    FILE _stderr;
    stdout_init(&_stdout);
    stderr_init(&_stderr);
    //    terminal_initialize();
    init_serial();
    printf("Hello, kernel World!\n\n");

    MemoryManager mman;
    mman_construct(&mman, mbt);

    KernelSection *ksects = ksection_getKsections();

    if (PRINT_DEBUG)
    {
        printf("MMap Addr: %p\n", mman.mbt->mmap_addr);

        size_t mmapLength = mman_getMemoryMapLength(&mman);
        printf("MMap Length: %d\n", (int)mmapLength);

        MemoryMap mmap[mmapLength];
        mman_getMemoryMap(&mman, mmap, mmapLength);
        for (int i = 0; i < mmapLength; ++i)
        {
            printf("Addr: %p; ", mmap[i].addr);
            printf("Len: %p; ", mmap[i].len);
            printf("Type: %d\n", mmap[i].type);
        }
        printf("\n");

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

    nvmos_test_runAllTests(
        ksects[KSECTION_SECTION_HEAP].addr,
        ksects[KSECTION_SECTION_HEAP].len);

    {
        InterruptDescriptor id;
        id.type = ID_TYPE_386_TRAP;
        id.isrAddr = (nvmos_ptr_t)&exceptionHandler;
        id.present = 1;
        id.privilegeLevel = 0;
        for (int i = 0; i < 0x20; ++i)
        {
            interruptDescriptor_Encode(
                &id,
                idtBuffer + 8 * i);
        }
        id.type = ID_TYPE_386_INT;
        id.isrAddr = (nvmos_ptr_t)&irqHandler;
        id.present = 1;
        id.privilegeLevel = 0;
        for (int i = 0x20; i < 0x30; ++i)
        {
            interruptDescriptor_Encode(
                &id,
                idtBuffer + 8 * i);
        }
        id.type = ID_TYPE_386_INT;
        id.isrAddr = (nvmos_ptr_t)&interruptHandler;
        id.present = 1;
        id.privilegeLevel = 0;
        for (int i = 0x30; i <= 0xFF; ++i)
        {
            interruptDescriptor_Encode(
                &id,
                idtBuffer + 8 * i);
        }
    }
    setIDT((uint32_t)idtBuffer, 256 * 8);
    PIC_remap(0x20, 0x28);
    for (int i = 0; i < 16; ++i)
    {
        IRQ_set_mask(i);
    }
    IRQ_clear_mask(0x04);

    asm("int $0x30");
    printf("After interrupt!\n");

    // Datalayer Allocator

    nvmos_dl_allocator_t datalayerBlockAllocator;
    nvmos_dl_alloc_createAllocator(
        &datalayerBlockAllocator,
        0x110000,
        0xbfed0000,
        0x1000);
    printf(
        "First Node Addr: %p\n",
        (datalayerBlockAllocator.head->content));

    nvmos_ptr_t firstAlloc =
        nvmos_dl_alloc_allocateBlocks(
            &datalayerBlockAllocator,
            5);
    nvmos_ptr_t secondAlloc =
        nvmos_dl_alloc_allocateBlocks(
            &datalayerBlockAllocator,
            5);

    printf("FirstAlloc: %p\n", firstAlloc);
    printf("SecondAlloc: %p\n", secondAlloc);

    nvmos_dl_alloc_deallocateBlocks(
        &datalayerBlockAllocator,
        firstAlloc,
        5);
    nvmos_dl_alloc_deallocateBlocks(
        &datalayerBlockAllocator,
        secondAlloc,
        5);

    nvmos_ptr_t thirdAlloc =
        nvmos_dl_alloc_allocateBlocks(
            &datalayerBlockAllocator,
            5);
    printf("ThirdAlloc: %p\n", thirdAlloc);

    printf("Sizeof meta_meta_t: %d\n", sizeof(meta_meta_t));

    while (1)
    {
        //        printf("%c",read_serial());
        printf("Serial: %d\n", read_serial());
    };
}
