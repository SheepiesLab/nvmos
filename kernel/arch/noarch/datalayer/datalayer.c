#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/meta.h>
#include <kernel/datalayer/allocator/redBlackTree/rbt.h>

struct nvmos_dl_datalayerHandle_struct
{
    nvmos_dl_datalayerMeta_t *datalayerMeta;
    nvmos_ptr_t startAddress;
    size_t blockCount;
    size_t allocationBlockSize;
    struct NVMOS_DL_meta *kroot;
    struct NVMOS_DL_meta *uroot;
    rbt_node_t *freeBlockList;
    uint64_t metaBlockList;
};

int datalayer_loadDatalayer(
    datalayer_t *datalayer,
    nvmos_ptr_t start)
{
    nvmos_dl_datalayerMeta_t *datalayerMeta =
        (nvmos_dl_datalayerMeta_t *)start;

    if (start == datalayerMeta->startAddress)
    {
        datalayer->datalayerMeta = datalayerMeta;
        datalayer->startAddress =
            (nvmos_ptr_t)(datalayerMeta->startAddress);
        datalayer->blockCount = datalayerMeta->blockCount;
        datalayer_allocationBlockSize =
            datalayerMeta->allocationBlockSize;
        datalayer->kroot =
            (struct NVMOS_DL_meta *)datalayerMeta->kroot;
        datalayer->uroot =
            (struct NVMOS_DL_meta *)datalayerMeta->uroot;
        datalayer->freeBlockList =
            (rbt_node_t)datalayerMeta->freeBlockList;
        datalayer->metaBlockList = datalayerMeta->metaBlockList;

        return 0;
    }
    return -1;
}

int datalayer_createDatalayer(
    datalayer_t *datalayer,
    nvmos_ptr_t start,
    size_t size,
    size_t allocationBlockSize)
{
    //TODO
}

size_t datalayer_getSizeOfDatalayer(
    datalayer_t *datalayer)
{
    if (datalayer == NULL)
        return 0;
    return datalayer->blockCount * datalayer->allocationBlockSize;
}

size_t datalayer_getPageSizeOfDatalayer(
    datalayer_t *datalayer)
{
    if (datalayer == NULL)
        return 0;
    return datalayer->allocationBlockSize;
}

int datalayer_resizeDatalayer(
    datalayer_t *datalayer,
    size_t newSize);

int datalayer_unloadDatalayer(
    datalayer_t *datalayer)
{
    datalayer->datalayerMeta = NULL;
    datalayer->startAddress = NULL;
    datalayer->blockCount = 0;
    datalayer_allocationBlockSize = 0;
    datalayer->kroot = NULL;
    datalayer->uroot = NULL;
    datalayer->freeBlockList = NULL;
    datalayer->metaBlockList = NULL;
}