#include <kernel/datalayer/ptrBlks.h>
#include <kernel/datalayer/allocator/allocator.h>

void ptrBlks_construct(
    ptrBlks_t *ptrBlks,
    ptrBlks_dataBlk_t *_1stBlk,
    ptrBlks_1stBlk_t *_1stPtrBlk,
    ptrBlks_2ndBlk_t *_2ndPtrBlk,
    ptrBlks_3rdBlk_t *_3rdPtrBlk)
{
    ptrBlks->_1stBlk = _1stBlk;
    ptrBlks->_1stPtrBlk = _1stPtrBlk;
    ptrBlks->_2ndPtrBlk = _2ndPtrBlk;
    ptrBlks->_3rdPtrBlk = _3rdPtrBlk;

    for (size_t i = 0; i < PTRBLKS_MAXSIZE; ++i)
    {
        if (ptrBlks_getDataBlkAt(ptrBlks, i) == NULL)
        {
            ptrBlks->size = i + 1;
            return;
        }
    }

    ptrBlks->size = PTRBLKS_MAXSIZE;
}

void ptrBlks_constructFromFileMeta(
    ptrBlks_t *ptrBlks,
    file_meta_t *fileMeta)
{
    ptrBlks->_1stBlk = fileMeta->_1stBlk;
    ptrBlks->_1stPtrBlk = fileMeta->_1stPtrBlk;
    ptrBlks->_2ndPtrBlk = fileMeta->_2ndPtrBlk;
    ptrBlks->_3rdPtrBlk = fileMeta->_3rdPtrBlk;

    for (size_t i = 0; i < PTRBLKS_MAXSIZE; ++i)
    {
        if (ptrBlks_getDataBlkAt(ptrBlks, i) == NULL)
        {
            ptrBlks->size = i + 1;
            return;
        }
    }

    ptrBlks->size = PTRBLKS_MAXSIZE;
}

void ptrBlks_saveToFileMeta(
    ptrBlks_t *ptrBlks,
    file_meta_t *fileMeta)
{
    fileMeta->_1stBlk = ptrBlks->_1stBlk;
    fileMeta->_1stPtrBlk = ptrBlks->_1stPtrBlk;
    fileMeta->_2ndPtrBlk = ptrBlks->_2ndPtrBlk;
    fileMeta->_3rdPtrBlk = ptrBlks->_3rdPtrBlk;
}

ptrBlks_dataBlk_t *ptrBlks_getDataBlkAt(
    ptrBlks_t *ptrBlks,
    size_t index)
{
    if (index == 0)
    {
        return ptrBlks->_1stBlk;
    }
    else if (index < 0x401)
    {
        index -= 1;
        return ptrBlks->_1stPtrBlk->_dataBlkPtrs[index];
    }
    else if (index < 0x100401)
    {
        index -= 0x401;
        size_t _2ndIndex = index >> 10;
        index = index % 0x400;

        ptrBlks_1stBlk_t *_1stBlk =
            (ptrBlks_1stBlk_t *)ptrBlks->_2ndPtrBlk->_1stBlkPtrs[_2ndIndex];
        return _1stBlk->_dataBlkPtrs[index];
    }
    else if (index < 0x40100401)
    {
        index -= 0x100401;
        size_t _3rdIndex = index >> 20;
        size_t _2ndIndex = (index >> 10) % 0x400;
        index = index % 0x400;

        ptrBlks_2ndBlk_t *_2ndBlk =
            (ptrBlks_2ndBlk_t *)ptrBlks->_3rdPtrBlk->_2ndBlkPtrs[_3rdIndex];
        ptrBlks_1stBlk_t *_1stBlk =
            (ptrBlks_1stBlk_t *)_2ndBlk->_1stBlkPtrs[_2ndIndex];
        return _1stBlk->_dataBlkPtrs[index];
    }
    else
        return NULL;
}

size_t ptrBlks_getSize(ptrBlks_t *ptrBlks)
{
    return ptrBlks->size;
}

int ptrBlks_pushBlks(
    ptrBlks_t *ptrBlks,
    nvmos_ptr_t blkSeg,
    size_t len,
    nvmos_dl_allocator_t *allocator)
{
    if ((ptrBlks->size + len) > PTRBLKS_MAXSIZE)
    {
        return -1;
    }

    if (ptrBlks->size == 1)
    {
        ptrBlks->_1stBlk = (ptrBlks_dataBlk_t *)blkSeg;
        len -= 1;
        blkSeg += 0x1000;
        ptrBlks->size += 1;

        if (len == 0)
            return 0;
    }

    if (ptrBlks->size < 0x401)
    {
        if (ptrBlks->size == 2)
        {
            nvmos_ptr_t newPtrBlk =
                nvmos_dl_alloc_allocateBlocks(allocator, 1);
            if (newPtrBlk == NULL)
                return -1;
            ptrBlks->_1stPtrBlk = (ptrBlks_1stBlk_t *)newPtrBlk;
        }

        size_t current = (ptrBlks->size - 1);
        while (current < 0x400)
        {
            ptrBlks->_1stPtrBlk->_dataBlkPtrs[current] = blkSeg;
            len -= 1;
            blkSeg += 0x1000;
            ptrBlks->size += 1;
            current = (ptrBlks->size - 1) % 0x400;

            if (len == 0)
                return 0;
        }
    }

    if (ptrBlks->size < 0x100401)
    {
        if (ptrBlks->size == 0x401)
        {
            nvmos_ptr_t newPtrBlk =
                nvmos_dl_alloc_allocateBlocks(allocator, 1);
            if (newPtrBlk == NULL)
                return -1;
            ptrBlks->_2ndPtrBlk = (ptrBlks_2ndBlk_t *)newPtrBlk;
        }

        size_t current = (ptrBlks->size - 0x401) % 0x400;
        size_t current1st = ((ptrBlks->size - 0x401) >> 10) % 0x400;
        while (current1st < 0x400)
        {
            ptrBlks_1stBlk_t *current1stPtr =
                ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st];
            if (current1stPtr == NULL)
            {
                nvmos_ptr_t newPtrBlk =
                    nvmos_dl_alloc_allocateBlocks(allocator, 1);
                if (newPtrBlk == NULL)
                    return -1;
                current1stPtr = (ptrBlks_1stBlk_t *)newPtrBlk;
                ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st] =
                    (ptrBlks_1stBlk_t *)newPtrBlk;
            }

            while (current < 0x400)
            {
                current1stPtr->_dataBlkPtrs[current] = blkSeg;
                len -= 1;
                blkSeg += 0x1000;
                ptrBlks->size += 1;
                current = (ptrBlks->size - 0x401) % 0x400;

                if (len == 0)
                    return 0;
            }

            current1st = ((ptrBlks->size - 0x401) >> 10) % 0x400;
        }
    }

    if (ptrBlks->size < 0x40100401)
    {
        if (ptrBlks->size == 0x100401)
        {
            nvmos_ptr_t newPtrBlk =
                nvmos_dl_alloc_allocateBlocks(allocator, 1);
            if (newPtrBlk == NULL)
                return -1;
            ptrBlks->_3rdPtrBlk = (ptrBlks_3rdBlk_t *)newPtrBlk;
        }

        size_t current = (ptrBlks->size - 0x100401) % 0x400;
        size_t current1st = ((ptrBlks->size - 0x100401) >> 10) % 0x400;
        size_t current2nd = ((ptrBlks->size - 0x100401) >> 20) % 0x400;

        while (current2nd < 0x400)
        {
            ptrBlks_2ndBlk_t *current2ndPtr =
                ptrBlks->_3rdPtrBlk->_2ndBlkPtrs[current2nd];
            if (current2ndPtr == NULL)
            {
                nvmos_ptr_t newPtrBlk =
                    nvmos_dl_alloc_allocateBlocks(allocator, 1);
                if (newPtrBlk == NULL)
                    return -1;
                current2ndPtr = (ptrBlks_2ndBlk_t *)newPtrBlk;
                ptrBlks->_3rdPtrBlk->_2ndBlkPtrs[current2nd] =
                    (ptrBlks_2ndBlk_t *)newPtrBlk;
            }
            while (current1st < 0x400)
            {
                ptrBlks_1stBlk_t *current1stPtr =
                    ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st];
                if (current1stPtr == NULL)
                {
                    nvmos_ptr_t newPtrBlk =
                        nvmos_dl_alloc_allocateBlocks(allocator, 1);
                    if (newPtrBlk == NULL)
                        return -1;
                    current1stPtr = (ptrBlks_1stBlk_t *)newPtrBlk;
                    ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st] =
                        (ptrBlks_1stBlk_t *)newPtrBlk;
                }

                while (current < 0x400)
                {
                    current1stPtr->_dataBlkPtrs[current] = blkSeg;
                    len -= 1;
                    blkSeg += 0x1000;
                    ptrBlks->size += 1;
                    current = (ptrBlks->size - 0x401) % 0x400;

                    if (len == 0)
                        return 0;
                }

                current1st = ((ptrBlks->size - 0x100401) >> 10) % 0x400;
            }

            current2nd = ((ptrBlks->size - 0x100401) >> 20) % 0x400;
        }
    }

    return -1;
}

int ptrBlks_popBlks(
    ptrBlks_t *ptrBlks,
    size_t len,
    nvmos_dl_allocator_t *allocator)
{
    if (len > ptrBlks->size)
    {
        len = ptrBlks->size;
    }

    if (ptrBlks->size >= PTRBLKS_MAXSIZE)
    {
        return -1;
    }

    if (ptrBlks->size >= 0x100401)
    {

        size_t current = (ptrBlks->size - 0x100401) % 0x400;
        size_t current1st = ((ptrBlks->size - 0x100401 - 1) >> 10) % 0x400;
        size_t current2nd = ((ptrBlks->size - 0x100401 - 1) >> 20) % 0x400;

        while (current2nd > 0)
        {
            ptrBlks_2ndBlk_t *current2ndPtr =
                (ptrBlks_2ndBlk_t *)ptrBlks->_3rdPtrBlk->_2ndBlkPtrs[current2nd];

            while (current1st > 0)
            {
                ptrBlks_1stBlk_t *current1stPtr =
                    (ptrBlks_1stBlk_t *)current2ndPtr->_1stBlkPtrs[current1st];

                nvmos_ptr_t blkSeg =
                    ptrBlks->_1stPtrBlk->_dataBlkPtrs[current] + 0x1000;
                size_t blkCount = 0;

                while (current > 0)
                {
                    current -= 1;
                    if (current1stPtr->_dataBlkPtrs[current] ==
                        blkSeg - 0x1000)
                    {
                        blkSeg = current1stPtr->_dataBlkPtrs[current];
                        blkCount += 1;
                    }
                    else
                    {
                        nvmos_dl_alloc_deallocateBlocks(
                            allocator,
                            blkSeg,
                            blkCount);
                        blkSeg = current1stPtr->_dataBlkPtrs[current];
                        blkCount = 1;
                    }

                    current1stPtr->_dataBlkPtrs[current] = NULL;
                    len -= 1;
                    ptrBlks->size -= 1;
                    current = (ptrBlks->size - 0x100401) % 0x400;

                    if (len == 0 || current == 0)
                    {
                        nvmos_dl_alloc_deallocateBlocks(
                            allocator,
                            blkSeg,
                            blkCount);
                    }

                    if (len == 0)
                        return 0;
                }

                nvmos_dl_alloc_deallocateBlocks(
                    allocator,
                    current1stPtr,
                    1);
                ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st] = NULL;
                current1st = ((ptrBlks->size - 0x100401 - 1) >> 10) % 0x400;
            }
            nvmos_dl_alloc_deallocateBlocks(
                allocator,
                current2ndPtr,
                1);
            ptrBlks->_3rdPtrBlk->_2ndBlkPtrs[current2nd] = NULL;
            current2nd = ((ptrBlks->size - 0x100401 - 1) >> 20) % 0x400;
        }
    }

    if (ptrBlks->size >= 0x401)
    {
        size_t current = (ptrBlks->size - 0x401) % 0x400;
        size_t current1st = ((ptrBlks->size - 0x401 - 1) >> 10) % 0x400;

        while (current1st > 0)
        {
            ptrBlks_1stBlk_t *current1stPtr =
                (ptrBlks_1stBlk_t *)ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st];

            nvmos_ptr_t blkSeg =
                ptrBlks->_1stPtrBlk->_dataBlkPtrs[current] + 0x1000;
            size_t blkCount = 0;

            while (current > 0)
            {
                current -= 1;
                if (current1stPtr->_dataBlkPtrs[current] ==
                    blkSeg - 0x1000)
                {
                    blkSeg = current1stPtr->_dataBlkPtrs[current];
                    blkCount += 1;
                }
                else
                {
                    nvmos_dl_alloc_deallocateBlocks(
                        allocator,
                        blkSeg,
                        blkCount);
                    blkSeg = current1stPtr->_dataBlkPtrs[current];
                    blkCount = 1;
                }

                current1stPtr->_dataBlkPtrs[current] = NULL;
                len -= 1;
                ptrBlks->size -= 1;
                current = (ptrBlks->size - 0x401) % 0x400;

                if (len == 0 || current == 0)
                {
                    nvmos_dl_alloc_deallocateBlocks(
                        allocator,
                        blkSeg,
                        blkCount);
                }

                if (len == 0)
                    return 0;
            }

            nvmos_dl_alloc_deallocateBlocks(
                allocator,
                current1stPtr,
                1);
            ptrBlks->_2ndPtrBlk->_1stBlkPtrs[current1st] = NULL;
            current1st = ((ptrBlks->size - 0x401 - 1) >> 10) % 0x400;
        }
    }

    if (ptrBlks->size > 1)
    {
        size_t current = (ptrBlks->size - 1) % 0x400;

        nvmos_ptr_t blkSeg =
            ptrBlks->_1stPtrBlk->_dataBlkPtrs[current] + 0x1000;
        size_t blkCount = 0;

        while (current > 0)
        {
            current -= 1;
            if (ptrBlks->_1stPtrBlk->_dataBlkPtrs[current] ==
                blkSeg - 0x1000)
            {
                blkSeg = ptrBlks->_1stPtrBlk->_dataBlkPtrs[current];
                blkCount += 1;
            }
            else
            {
                nvmos_dl_alloc_deallocateBlocks(
                    allocator,
                    blkSeg,
                    blkCount);
                blkSeg = ptrBlks->_1stPtrBlk->_dataBlkPtrs[current];
                blkCount = 1;
            }

            ptrBlks->_1stPtrBlk->_dataBlkPtrs[current] = NULL;
            len -= 1;
            ptrBlks->size -= 1;
            current = (ptrBlks->size - 1) % 0x400;

            if (len == 0 || current == 0)
            {
                nvmos_dl_alloc_deallocateBlocks(
                    allocator,
                    blkSeg,
                    blkCount);
            }

            if (len == 0)
                return 0;
        }
    }

    if (ptrBlks->size == 1)
    {
        len -= 1;
        ptrBlks->size -= 1;
        nvmos_dl_alloc_deallocateBlocks(allocator, ptrBlks->_1stBlk, 1);
        ptrBlks->_1stBlk = NULL;

        if (len == 0)
            return 0;
    }

    return -1;
}
