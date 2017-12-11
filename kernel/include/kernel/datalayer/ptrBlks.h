#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/meta.h>

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

struct ptrBlks
{
    ptrBlks_dataBlk_t *_1stBlk;   //Hold 1 block
    ptrBlks_1stBlk_t *_1stPtrBlk; //Hold 1k blocks
    ptrBlks_2ndBlk_t *_2ndPtrBlk; //Hold 1M blocks
    ptrBlks_3rdBlk_t *_3rdPtrBlk; //Hold 1G blocks

    size_t size;
};
typedef struct ptrBlks ptrBlks_t;

#define PTRBLKS_MAX 0x40100401

void ptrBlks_construct(
    ptrBlks_t *ptrBlks,
    ptrBlks_dataBlk_t *_1stBlk,
    ptrBlks_1stBlk_t *_1stPtrBlk,
    ptrBlks_2ndBlk_t *_2ndPtrBlk,
    ptrBlks_3rdBlk_t *_3rdPtrBlk);

void ptrBlks_constructFromFileMeta(
    ptrBlks_t *ptrBlks,
    file_meta_t *fileMeta);

void ptrBlks_saveToFileMeta(
    ptrBlks_t *ptrBlks,
    file_meta_t *fileMeta);

ptrBlks_dataBlk_t *ptrBlks_getDataBlkAt(
    ptrBlks_t *ptrBlks,
    size_t index);

size_t ptrBlks_getSize(ptrBlks_t *ptrBlks);

int ptrBlks_pushBlks(
    ptrBlks_t *ptrBlks,
    nvmos_ptr_t blkSeg,
    size_t len,
    nvmos_dl_allocator_t *allocator);

int ptrBlks_popBlks(
    ptrBlks_t *ptrBlks,
    size_t len,
    nvmos_dl_allocator_t *allocator);
