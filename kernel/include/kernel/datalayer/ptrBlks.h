#include <kernel/datalayer/datalayer.h>

struct ptrBlks_3rdBlk
{
    uint32_t _2ndBlkPtrs[1024];
};
typedef struct ptrBlks_3rdBlk ptrBlks_3rdBlk_t;

struct ptrBlks_2ndBlk
{
    uint32_t _1stBlkPtrs[1024];
};
typedef struct ptrBlks_2ndBlk ptrBlks_2ndBlk_t;

struct ptrBlks_1stBlk
{
    uint32_t _dataBlkPtrs[1024];
};
typedef struct ptrBlks_1stBlk ptrBlks_1stBlk_t;

struct ptrBlks_dataBlk
{
    uint8_t dataBuff[4096];
};
typedef struct ptrBlks_dataBlk ptrBlks_dataBlk_t;

struct ptrBlks_dataBlkIter
{
    ptrBlks_dataBlk_t *_1stBlk;   //Hold 1 block
    ptrBlks_1stBlk_t *_1stPtrBlk; //Hold 1k blocks
    ptrBlks_2ndBlk_t *_2ndPtrBlk; //Hold 1M blocks
    ptrBlks_3rdBlk_t *_3rdPtrBlk; //Hold 1G blocks
    size_t pos;
};
typedef struct ptrBlks_dataBlkIter ptrBlks_dataBlkIter_t;

int ptrBlks_pushBlk(
    ptrBlks_dataBlkIter_t *iter,
    nvmos_ptr_t blk);

int ptrBlks_pushBlks(
    ptrBlks_dataBlkIter_t *iter,
    nvmos_ptr_t blkSeg,
    size_t len);

int ptrBlks_popBlks(
    ptrBlks_dataBlkIter_t *iter,
    size_t len);

size_t ptrBlks_count(
    ptrBlks_dataBlkIter_t *iter);

void ptrBLks_initIter(
    ptrBlks_dataBlkIter_t *iter,
    ptrBlks_dataBlk_t *_1stBlk,
    ptrBlks_1stBlk_t *_1stPtrBlk,
    ptrBlks_2ndBlk_t *_2ndPtrBlk,
    ptrBlks_3rdBlk_t *_3rdPtrBlk);

size_t ptrBlks_getPos(ptrBlks_dataBlkIter_t *iter);

nvmos_ptr_t ptrBlks_goto(ptrBlks_dataBlkIter_t *iter, size_t pos);

nvmos_ptr_t ptrBlks_this(ptrBlks_dataBlkIter_t *iter);

nvmos_ptr_t ptrBlks_next(ptrBlks_dataBlkIter_t *iter);

nvmos_ptr_t ptrBlks_prev(ptrBlks_dataBlkIter_t *iter);

nvmos_ptr_t ptrBlks_first(ptrBlks_dataBlkIter_t *iter);

nvmos_ptr_t ptrBlks_last(ptrBlks_dataBlkIter_t *iter);
