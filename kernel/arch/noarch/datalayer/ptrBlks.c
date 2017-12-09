#include <kernel/datalayer/ptrBlks.h>
#include <kernel/datalayer/allocator/allocator.h>

//TODO push&pop

int ptrBlks_pushBlk(
    ptrBlks_dataBlkIter_t *iter,
    nvmos_ptr_t blk)
{
    if (iter->_1stBlk == NULL)
    {
        iter->_1stBlk = ptrBlks_dataBlk_t *blk;
    }

    if (iter->_1stPtrBlk == NULL)
    {
        
    }

    size_t i = 0;
    for (
        ;
        i < 0x400 || iter->_1stPtrBlk->_dataBlkPtrs[i] != NULL;
        ++i)
        ;

    if (i != 0x400)
        return result + i;

    result += 0x400;

    for (size_t j = 0; j < 0x400; ++j)
    {
        ptrBlks_1stBlk_t *_1stBlk =
            (ptrBlks_1stBlk_t *)iter->_2ndPtrBlk->_1stBlkPtrs[j];
        for (
            i = 0;
            i < 0x400 || _1stBlk->_dataBlkPtrs[i] != NULL;
            ++i)
            ;

        if (i != 0x400)
            return result + i + j * 0x400;
    }

    result += 0x100000;

    for (size_t k = 0; k < 0x400; ++k)
    {
        ptrBlks_2ndBlk_t *_2ndBlk =
            (ptrBlks_2ndBlk_t *)iter->_3rdPtrBlk->_2ndBlkPtrs[k];
        for (size_t j = 0; j < 0x400; ++j)
        {
            ptrBlks_1stBlk_t *_1stBlk =
                (ptrBlks_1stBlk_t *)_2ndBlk->_1stBlkPtrs[j];
            for (
                i = 0;
                i < 0x400 || _1stBlk->_dataBlkPtrs[i] != NULL;
                ++i)
                ;

            if (i != 0x400)
                return result + i + j * 0x400 + k * 0x100000;
        }
    }

    return result + 0x40000000;
}

int ptrBlks_popBlks(
    ptrBlks_dataBlkIter_t *iter,
    size_t len)
{
}

size_t ptrBlks_count(
    ptrBlks_dataBlkIter_t *iter)
{
    size_t result = 0;

    if (iter->_1stBlk == NULL)
        return result;

    result += 1;

    if (iter->_1stPtrBlk == NULL)
        return result;

    size_t i = 0;
    for (
        ;
        i < 0x400 || iter->_1stPtrBlk->_dataBlkPtrs[i] != NULL;
        ++i)
        ;

    if (i != 0x400)
        return result + i;

    result += 0x400;

    for (size_t j = 0; j < 0x400; ++j)
    {
        ptrBlks_1stBlk_t *_1stBlk =
            (ptrBlks_1stBlk_t *)iter->_2ndPtrBlk->_1stBlkPtrs[j];
        for (
            i = 0;
            i < 0x400 || _1stBlk->_dataBlkPtrs[i] != NULL;
            ++i)
            ;

        if (i != 0x400)
            return result + i + j * 0x400;
    }

    result += 0x100000;

    for (size_t k = 0; k < 0x400; ++k)
    {
        ptrBlks_2ndBlk_t *_2ndBlk =
            (ptrBlks_2ndBlk_t *)iter->_3rdPtrBlk->_2ndBlkPtrs[k];
        for (size_t j = 0; j < 0x400; ++j)
        {
            ptrBlks_1stBlk_t *_1stBlk =
                (ptrBlks_1stBlk_t *)_2ndBlk->_1stBlkPtrs[j];
            for (
                i = 0;
                i < 0x400 || _1stBlk->_dataBlkPtrs[i] != NULL;
                ++i)
                ;

            if (i != 0x400)
                return result + i + j * 0x400 + k * 0x100000;
        }
    }

    return result + 0x40000000;
}

void ptrBLks_initIter(
    ptrBlks_dataBlkIter_t *iter,
    ptrBlks_dataBlk_t *_1stBlk,
    ptrBlks_1stBlk_t *_1stPtrBlk,
    ptrBlks_2ndBlk_t *_2ndPtrBlk,
    ptrBlks_3rdBlk_t *_3rdPtrBlk)
{
    iter->pos = 0;
    iter->_1stBlk = _1stBlk;
    iter->_1stPtrBlk = _1stPtrBlk;
    iter->_2ndPtrBlk = _2ndPtrBlk;
    iter->_3rdPtrBlk = _3rdPtrBlk;
}

size_t ptrBlks_getPos(ptrBlks_dataBlkIter_t *iter)
{
    return iter->pos;
}

nvmos_ptr_t ptrBlks_goto(ptrBlks_dataBlkIter_t *iter, size_t pos)
{
    iter->pos = pos;
    return ptrBlks_this(iter);
}

nvmos_ptr_t ptrBlks_this(ptrBlks_dataBlkIter_t *iter)
{
    size_t pos = iter->pos;
    uint32_t _1stIndex, _2ndIndex, _3rdIndex;
    if (pos == 0)
        return (nvmos_ptr_t)iter->_1stBlk;
    else if (pos < 0x401)
    {
        _1stIndex = pos - 1;
        return (nvmos_ptr_t)(iter->_1stPtrBlk->_dataBlkPtrs[_1stIndex]);
    }
    else if (pos < 0x100401)
    {
        _1stIndex = (pos - 0x401) % 0x400;
        _2ndIndex = (pos - 0x401) >> 10;
        ptrBlks_1stBlk_t *_1stBlk =
            (ptrBlks_1stBlk_t *)(iter->_2ndPtrBlk->_1stBlkPtrs[_2ndIndex]);
        return (nvmos_ptr_t)(_1stBlk->_dataBlkPtrs[_1stIndex]);
    }
    else if (pos < 0x40100401)
    {
        _1stIndex = (pos - 0x100401) % 0x400;
        _2ndIndex = ((pos - 0x100401) >> 10) % 0x400;
        _3rdIndex = (pos - 0x100401) >> 20;
        ptrBlks_2ndBlk_t *_2ndBlk =
            (ptrBlks_2ndBlk_t *)(iter->_3rdPtrBlk->_2ndBlkPtrs[_3rdIndex]);
        ptrBlks_1stBlk_t *_1stBlk =
            (ptrBlks_1stBlk_t *)(_2ndBlk->_1stBlkPtrs[_2ndIndex]);
        return (nvmos_ptr_t)(_1stBlk->_dataBlkPtrs[_1stIndex]);
    }
    else
        return NULL;
}

nvmos_ptr_t ptrBlks_next(ptrBlks_dataBlkIter_t *iter)
{
    ++(iter->pos);
    return ptrBlks_this(iter);
}

nvmos_ptr_t ptrBlks_prev(ptrBlks_dataBlkIter_t *iter)
{
    if (iter->pos == 0)
    {
        return NULL;
    }
    --(iter->pos);
    return ptrBlks_this(iter);
}

nvmos_ptr_t ptrBlks_first(ptrBlks_dataBlkIter_t *iter)
{
    return (nvmos_ptr_t)(iter->_1stBlk);
}

nvmos_ptr_t ptrBlks_last(ptrBlks_dataBlkIter_t *iter)
{
    size_t pos = iter->pos;
    nvmos_ptr_t res;
    iter->pos = ptrBlks_count(iter) - 1;
    res = ptrBlks_this(iter);
    iter->pos = pos;
    return res;
}