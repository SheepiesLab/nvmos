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

void kernel_main(multiboot_info_t *mbt) {

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

    if (PRINT_DEBUG) {
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

        // Test Heap
        int *testIntPtr = (int *) heap_malloc(&(mman.heap),
                                              sizeof(int));
        int *testIntPtr2 = (int *) heap_malloc(&(mman.heap),
                                               sizeof(int));
        int *testIntArrPtr = (int *) heap_calloc(
                &mman.heap,
                100,
                sizeof(int));
        int *testIntPtr3 = (int *) heap_malloc(&(mman.heap),
                                               sizeof(int));
        heap_free(&mman.heap, (kptr_t) testIntPtr2);
        int *testIntPtr4 = (int *) heap_malloc(&(mman.heap),
                                               sizeof(int));

        *testIntPtr = 0;
        for (int i = 0; i < 100; ++i) testIntArrPtr[i] = 0;
        *testIntPtr3 = 0;
        *testIntPtr4 = 0;

        printf("heapManStart: 0x%p\n", mman.heap.heapStart);
        printf("heapManEnd: 0x%p\n", mman.heap.heapEnd);
        printf("testPtr1: 0x%p\n", testIntPtr);
        printf("testPtr2: 0x%p\n", testIntPtr2);
        printf("testPtrA: 0x%p\n", testIntArrPtr);
        printf("testPtr3: 0x%p\n", testIntPtr3);
        printf("testPtr4: 0x%p\n", testIntPtr4);

        uint16_t *com1 = (kptr_t) 0x400;
        printf("com1: 0x%p\n", *com1);
    }

    {
        InterruptDescriptor id;
        id.type = ID_TYPE_386_TRAP;
        id.isrAddr = (kptr_t) &exceptionHandler;
        id.present = 1;
        id.privilegeLevel = 0;
        for (int i = 0; i < 0x20; ++i) {
            interruptDescriptor_Encode(
                    &id,
                    idtBuffer + 8 * i);
        }
        id.type = ID_TYPE_386_INT;
        id.isrAddr = (kptr_t) &irqHandler;
        id.present = 1;
        id.privilegeLevel = 0;
        for (int i = 0x20; i < 0x30; ++i) {
            interruptDescriptor_Encode(
                    &id,
                    idtBuffer + 8 * i);
        }
        id.type = ID_TYPE_386_INT;
        id.isrAddr = (kptr_t) &interruptHandler;
        id.present = 1;
        id.privilegeLevel = 0;
        for (int i = 0x30; i <= 0xFF; ++i) {
            interruptDescriptor_Encode(
                    &id,
                    idtBuffer + 8 * i);
        }
    }
    setIDT((uint32_t) idtBuffer, 256 * 8);
    PIC_remap(0x20, 0x28);
    for (int i = 0;i < 16; ++i){
        IRQ_set_mask(i);
    }
    IRQ_clear_mask(0x04);

    asm("int $0x30");
    printf("After interrupt!\n");
    while(1){
        printf("%c",read_serial());
    };
}
