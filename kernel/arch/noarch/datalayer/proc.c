#include <kernel/datalayer/proc.h>
#include <kernel/datalayer/ptrBlks.h>

int proc_createProc(
    proc_meta_t *meta,
    nvmos_dl_allocator_t *alloc)
{
    meta->pageDir =
        (uint32_t)nvmos_dl_alloc_allocateBlocks(alloc, 1);
    if (meta->pageDir == NULL)
        return -1;
    memset(meta->pageDir, 0, 0x1000);
    return 0;
}

int proc_allocBlks(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    if (procAddr % 0x1000 != 0)
    {
        procAddr = procAddr & 0xFFFFF000;
        procAddr += 0x1000;
    }

    size_t i = procAddr;
    size_t term = procAddr + len * 0x1000;
    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t pageDirEntry =
            pageDir->pageDirEntries[pageDirIndex];

        if (pageDirEntry & PAGEDIR_FILEMAPPED ||
            pageDirEntry & PAGEDIR_KERNEL)
        {
            return -1;
        }

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);
        if (!(pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            i = i & 0xFFC00000;
            i += 1 << 22;
            i -= 0x1000;
            continue;
        }

        uint32_t pageTableEntry =
            pageTable->pageTableEntries[pageTableIndex];

        if (pageTableEntry & PAGETABLE_PRESENT)
        {
            return -1;
        }
    }

    nvmos_ptr_t newBlks =
        nvmos_dl_alloc_allocateBlocks(alloc, len);
    if (newBlks == NULL)
        return -1;

    i = procAddr;
    term = procAddr + len * 0x1000;
    pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t *pageDirEntry =
            &(pageDir->pageDirEntries[pageDirIndex]);
        pageTable_t *pageTable =
            (pageTable_t *)(*pageDirEntry &
                            PAGEDIR_ADDR_MASK);

        if (pageTable == NULL)
        {
            nvmos_ptr_t newTableBlk =
                nvmos_dl_alloc_allocateBlocks(alloc, 1);
            *pageDirEntry =
                (uint32_t)newTableBlk & PAGEDIR_ADDR_MASK;
            pageTable_t *pageTable =
                (pageTable_t *)(*pageDirEntry &
                                PAGEDIR_ADDR_MASK);
        }
        if (!(*pageDirEntry & PAGEDIR_PRESENT))
        {
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_PRESENT;
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_RW;
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_USER;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_WRITETHROUGH;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_NOCACHE;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_ACCESSED;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_BIGPAGE;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_FILEMAPPED;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_KERNEL;
        }

        uint32_t *pageTableEntry =
            &(pageTable->pageTableEntries[pageTableIndex]);
        *pageTableEntry = newBlks & PAGEDIR_ADDR_MASK;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_PRESENT;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_RW;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_USER;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_WRITETHROUGH;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_NOCACHE;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_ACCESSED;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_DIRTY;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_GLOBAL;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_FILEMAPPED;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_KERNEL;
    }

    return 0;
}

int proc_freeBlks(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    if (procAddr % 0x1000 != 0)
    {
        procAddr = procAddr & 0xFFFFF000;
        procAddr += 0x1000;
    }

    size_t i = procAddr;
    size_t term = procAddr + len * 0x1000;
    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t pageDirEntry =
            pageDir->pageDirEntries[pageDirIndex];

        if (pageDirEntry & PAGEDIR_FILEMAPPED ||
            pageDirEntry & PAGEDIR_KERNEL)
        {
            return -1;
        }

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);
        if (!(pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            return -1;
        }

        uint32_t pageTableEntry =
            pageTable->pageTableEntries[pageTableIndex];

        if (!(pageTableEntry & PAGETABLE_PRESENT) ||
            pageTableEntry & PAGETABLE_FILEMAPPED ||
            pageTableEntry & PAGETABLE_KERNEL)
        {
            return -1;
        }
    }

    i = procAddr;
    term = procAddr + len * 0x1000;
    pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);

        uint32_t *pageTableEntry =
            &(pageTable->pageTableEntries[pageTableIndex]);

        nvmos_dl_alloc_deallocateBlocks(
            alloc,
            *pageTableEntry & PAGEDIR_ADDR_MASK,
            1);
        *pageTableEntry = 0;
    }

    return 0;
}

int proc_mapFile(
    proc_meta_t *meta,
    file_meta_t *file,
    size_t blkPos,
    size_t len,
    uint8_t rwx,
    nvmos_ptr_t procAddr,
    nvmos_dl_allocator_t *alloc)
{
    if (procAddr % 0x1000 != 0)
    {
        procAddr = procAddr & 0xFFFFF000;
        procAddr += 0x1000;
    }

    size_t i = procAddr;
    size_t term = procAddr + len * 0x1000;
    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t pageDirEntry =
            pageDir->pageDirEntries[pageDirIndex];

        if (pageDirEntry & PAGEDIR_KERNEL)
        {
            return -1;
        }

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);
        if (!(pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            i = i & 0xFFC00000;
            i += 1 << 22;
            i -= 0x1000;
            continue;
        }

        uint32_t pageTableEntry =
            pageTable->pageTableEntries[pageTableIndex];

        if (pageTableEntry & PAGETABLE_PRESENT)
        {
            return -1;
        }
    }

    nvmos_ptr_t blkPtrs[len];
    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(&ptrBlks, file);
    for (size_t j = 0; j < len; ++j)
    {
        blkPtrs[j] =
            (nvmos_ptr_t)ptrBlks_getDataBlkAt(&ptrBlks, blkPos + j);
        if (blkPtrs[j] == NULL)
        {
            return -1;
        }
    }

    i = procAddr;
    term = procAddr + len * 0x1000;
    pageDir = (pageDir_t *)meta->pageDir;
    for (size_t j = 0; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t *pageDirEntry =
            &(pageDir->pageDirEntries[pageDirIndex]);
        pageTable_t *pageTable =
            (pageTable_t *)(*pageDirEntry &
                            PAGEDIR_ADDR_MASK);

        if (pageTable == NULL)
        {
            nvmos_ptr_t newTableBlk =
                nvmos_dl_alloc_allocateBlocks(alloc, 1);
            *pageDirEntry =
                (uint32_t)newTableBlk & PAGEDIR_ADDR_MASK;
            pageTable_t *pageTable =
                (pageTable_t *)(*pageDirEntry &
                                PAGEDIR_ADDR_MASK);
        }
        if (!(*pageDirEntry & PAGEDIR_PRESENT))
        {
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_PRESENT;
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_RW;
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_USER;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_WRITETHROUGH;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_NOCACHE;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_ACCESSED;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_BIGPAGE;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_FILEMAPPED;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_KERNEL;
        }

        uint32_t *pageTableEntry =
            &(pageTable->pageTableEntries[pageTableIndex]);
        *pageTableEntry = blkPtrs[j] & PAGEDIR_ADDR_MASK;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_PRESENT;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_RW;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_USER;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_WRITETHROUGH;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_NOCACHE;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_ACCESSED;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_DIRTY;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_GLOBAL;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_FILEMAPPED;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_KERNEL;

        ++j;
    }

    return 0;
}

int proc_unmapFile(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc);
{
    if (procAddr % 0x1000 != 0)
    {
        procAddr = procAddr & 0xFFFFF000;
        procAddr += 0x1000;
    }

    size_t i = procAddr;
    size_t term = procAddr + len * 0x1000;
    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t pageDirEntry =
            pageDir->pageDirEntries[pageDirIndex];

        if (pageDirEntry & PAGEDIR_KERNEL)
        {
            return -1;
        }

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);
        if (!(pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            return -1;
        }

        uint32_t pageTableEntry =
            pageTable->pageTableEntries[pageTableIndex];

        if (!(pageTableEntry & PAGETABLE_PRESENT) ||
            pageTableEntry & PAGETABLE_KERNEL ||
            !(pageTableEntry & PAGETABLE_FILEMAPPED))
        {
            return -1;
        }
    }

    i = procAddr;
    term = procAddr + len * 0x1000;
    pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);

        uint32_t *pageTableEntry =
            &(pageTable->pageTableEntries[pageTableIndex]);

        *pageTableEntry = 0;
    }

    return 0;
}

int proc_mapKernel(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    nvmos_ptr_t kAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    if (procAddr % 0x1000 != 0)
    {
        procAddr = procAddr & 0xFFFFF000;
        procAddr += 0x1000;
    }

    size_t i = procAddr;
    size_t term = procAddr + len * 0x1000;
    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t pageDirEntry =
            pageDir->pageDirEntries[pageDirIndex];

        if (pageDirEntry & PAGEDIR_FILEMAPPED)
        {
            return -1;
        }

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);
        if (!(pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            i = i & 0xFFC00000;
            i += 1 << 22;
            i -= 0x1000;
            continue;
        }

        uint32_t pageTableEntry =
            pageTable->pageTableEntries[pageTableIndex];

        if (pageTableEntry & PAGETABLE_PRESENT)
        {
            return -1;
        }
    }

    nvmos_ptr_t newBlks = kAddr;

    i = procAddr;
    term = procAddr + len * 0x1000;
    pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t *pageDirEntry =
            &(pageDir->pageDirEntries[pageDirIndex]);
        pageTable_t *pageTable =
            (pageTable_t *)(*pageDirEntry &
                            PAGEDIR_ADDR_MASK);

        if (pageTable == NULL)
        {
            nvmos_ptr_t newTableBlk =
                nvmos_dl_alloc_allocateBlocks(alloc, 1);
            *pageDirEntry =
                (uint32_t)newTableBlk & PAGEDIR_ADDR_MASK;
            pageTable_t *pageTable =
                (pageTable_t *)(*pageDirEntry &
                                PAGEDIR_ADDR_MASK);
        }
        if (!(*pageDirEntry & PAGEDIR_PRESENT))
        {
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_PRESENT;
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_RW;
            *pageDirEntry =
                *pageDirEntry | PAGEDIR_USER;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_WRITETHROUGH;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_NOCACHE;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_ACCESSED;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_BIGPAGE;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_FILEMAPPED;
            *pageDirEntry =
                *pageDirEntry & ~PAGEDIR_KERNEL;
        }

        uint32_t *pageTableEntry =
            &(pageTable->pageTableEntries[pageTableIndex]);
        *pageTableEntry = newBlks & PAGEDIR_ADDR_MASK;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_PRESENT;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_RW;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_USER;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_WRITETHROUGH;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_NOCACHE;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_ACCESSED;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_DIRTY;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_GLOBAL;
        *pageTableEntry =
            *pageTableEntry & ~PAGETABLE_FILEMAPPED;
        *pageTableEntry =
            *pageTableEntry | PAGETABLE_KERNEL;
    }

    return 0;
}

int proc_unmapKernel(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    nvmos_ptr_t kAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    if (procAddr % 0x1000 != 0)
    {
        procAddr = procAddr & 0xFFFFF000;
        procAddr += 0x1000;
    }

    size_t i = procAddr;
    size_t term = procAddr + len * 0x1000;
    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        uint32_t pageDirEntry =
            pageDir->pageDirEntries[pageDirIndex];

        if (pageDirEntry & PAGEDIR_FILEMAPPED)
        {
            return -1;
        }

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);
        if (!(pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            return -1;
        }

        uint32_t pageTableEntry =
            pageTable->pageTableEntries[pageTableIndex];

        if (!(pageTableEntry & PAGETABLE_PRESENT) ||
            pageTableEntry & PAGETABLE_FILEMAPPED ||
            !(pageTableEntry & PAGETABLE_KERNEL))
        {
            return -1;
        }
    }

    i = procAddr;
    term = procAddr + len * 0x1000;
    pageDir = (pageDir_t *)meta->pageDir;
    for (; i < term; i += 0x1000)
    {
        size_t pageDirIndex = i >> 22;
        size_t pageTableIndex = (i >> 12) % 0x400;

        pageTable_t *pageTable =
            (pageTable_t *)((pageDir->pageDirEntries
                                 [pageDirIndex]) &
                            PAGEDIR_ADDR_MASK);

        uint32_t *pageTableEntry =
            &(pageTable->pageTableEntries[pageTableIndex]);

        *pageTableEntry = 0;
    }

    return 0;
}

int proc_removeProc(
    proc_meta_t *meta,
    nvmos_dl_allocator_t *alloc)
{
    nvmos_ptr_t procAddr = 0;
    size_t len = 0x400;

    pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
    for (size_t j = 0; j < len; ++j)
    {
        uint32_t *pageDirEntry =
            &(pageDir->pageDirEntries[j]);
        pageTable_t *pageTable =
            (pageTable_t *)(pageDirEntry & PAGEDIR_ADDR_MASK);

        if (!(*pageDirEntry & PAGEDIR_PRESENT) ||
            pageTable == NULL)
        {
            continue;
        }

        for (size_t k = 0; k < 0x400; ++k)
        {
            uint32_t *pageTableEntry =
                &(pageTable->pageTableEntries[k]);
            nvmos_ptr_t ptr = *pageTableEntry & PAGEDIR_ADDR_MASK;
            if (ptr == NULL)
                continue;
            if (!(*pageTableEntry & PAGETABLE_PRESENT))
                continue;

            if (*pageTableEntry & PAGETABLE_FILEMAPPED)
            {
                *pageTableEntry = 0;
            }
            else if (*pageTableEntry & PAGETABLE_KERNEL)
            {
                *pageTableEntry = 0;
            }
            else
            {
                nvmos_dl_alloc_deallocateBlocks(alloc, ptr, 1);
            }
        }

        nvmos_dl_alloc_deallocateBlocks(alloc, (nvmos_ptr_t)pageTable, 1);
    }

    return 0;
}