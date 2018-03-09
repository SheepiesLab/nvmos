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
#include <kernel/mman/paging.h>
#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/meta.h>
#include <kernel/datalayer/directory.h>
#include <kernel/datalayer/datalayer.h>

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

    //Kernel Code Sections
    KernelSection *ksects = ksection_getKsections();
    if (PRINT_DEBUG)
    {
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

    // Memory Manager
    MemoryManager mman;
    mman_construct(&mman, mbt);
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
    }

    // Tests
    // printf("\n");
    // printf("\n");
    // printf("Test Memory Address: %p\n",
    //        (nvmos_ptr_t)(ksects[KSECTION_SECTION_HEAP].addr +
    //                      ksects[KSECTION_SECTION_HEAP].len));
    // printf("Test Memory Size: %p\n", 0x40000000);
    // nvmos_test_runAllTests(
    //     (nvmos_ptr_t)(ksects[KSECTION_SECTION_HEAP].addr +
    //                   ksects[KSECTION_SECTION_HEAP].len),
    //     0x40000000);
    // printf("\n");
    // printf("\n");

    // Interrupt Setup
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

    // Init Datalayer
    nvmos_ptr_t dlStart =
        (ksects[KSECTION_SECTION_HEAP].addr +
         ksects[KSECTION_SECTION_HEAP].len);
    dlStart += 0x1000;
    dlStart &= 0xfffff000;
    size_t dlSize = 0x40000000;
    size_t dlBlkSize = 0x1000;
    nvmos_dl_datalayerMeta_t *dlMeta;
    nvmos_dl_allocator_t allocator;
    if ((dlMeta = datalayer_createDatalayer(
             dlStart, dlSize, dlBlkSize, &allocator)) == NULL ||
        (nvmos_ptr_t)dlMeta != dlStart)
    {
        printf("Datalayer Creation Failed!\n");
    }
    else
    {
        printf("DL start:       0x%p\t",
               (uint64_t)dlStart);
        printf("DL length:      0x%p\n",
               (uint64_t)dlSize);
    }

    // Add a test file to uroot
    meta_meta_t *uroot = datalayer_getURoot(dlMeta);
    if (uroot == NULL)
    {
        printf("U Root is corrupted...\n");
        goto endProc;
    }
    file_meta_t *urootDir = &(uroot->metaContent.fileMeta);
    meta_meta_t *file0 = meta_getNextFreeMeta(
        (meta_metaBlk_t **)&(dlMeta->metaBlockList),
        &allocator);

    meta_setFile(file0);
    if (
        dir_addFileRef(urootDir, "file0", file0, &allocator) ==
        dir_fileRefId_inval)
    {
        printf("New file ref bad: file0...\n");
        goto endProc;
    }
    file_meta_t *file0Meta = &(file0->metaContent.fileMeta);
    file0Meta->refCount = 0;
    file0Meta->fileSize = 0;
    file0Meta->blkSize = 0;
    file0Meta->_1stBlk = NULL;
    file0Meta->_1stPtrBlk = NULL;
    file0Meta->_2ndPtrBlk = NULL;
    file0Meta->_3rdPtrBlk = NULL;
    for (int i = 0; i < 0x1000; i += 8)
        if (file_append(file0Meta, "JOJOJOJO", 8, &allocator) != 8)
        {
            printf("Append file bad: file0...\n");
            goto endProc;
        }

    // Add a process to kroot
    meta_meta_t *kroot = datalayer_getKRoot(dlMeta);
    if (kroot == NULL)
    {
        printf("K Root is corrupted...\n");
        goto endProc;
    }
    file_meta_t *krootDir = &(kroot->metaContent.fileMeta);

    meta_meta_t *proc0 = meta_getNextFreeMeta(
        (meta_metaBlk_t **)&(dlMeta->metaBlockList),
        &allocator);

    meta_setProc(proc0);
    if (dir_addFileRef(krootDir, "proc0", proc0, &allocator) == dir_fileRefId_inval)
    {
        printf("New file ref bad: proc0...\n");
        goto endProc;
    }
    proc_meta_t *proc0Meta = &(proc0->metaContent.processMeta);
    if (proc_createProc(proc0Meta, &allocator))
    {
        printf("Error creating proc0\n");
        goto endProc;
    }
    if (proc_mapKernel(proc0Meta, 0, 0, 0x114000 / 0x1000, &allocator))
    {
        printf("Error mapping kernel memory to proc0\n");
        goto endProc;
    }
    nvmos_ptr_t fileMap[2];
    int fileMapLen = file_getMap(file0Meta, fileMap, 0, 2);
    if (fileMapLen != 2)
    {
        printf("Error getting ptr map of file0\n");
        goto endProc;
    }
    for (int i = 0; i < fileMapLen; ++i)
        printf("File map ptr %d: 0x%p\n", i, (uint64_t)(fileMap[i]));
    if (proc_mapFile(
            proc0Meta,
            file0Meta,
            0,
            fileMapLen,
            0,
            0x40000000,
            &allocator))
    {
        printf("Error mapping file0 to proc0\n\n");
        goto endProc;
    }

    nvmos_pagingOn(proc0Meta->pageDir);
    // printf("Test Mem 1: %p\n", *(uint32_t *)0x40000000);
    // printf("Test Mem 2: %p\n", *(uint32_t *)0x40001ffc);

#undef allocTest
endProc:

    while (1)
    {
        //        printf("%c",read_serial());
        printf("Serial: %d\n", read_serial());
    };
}
