#include <kernel/datalayer/datalayer.h>

int datalayer_loadDatalayer(nvmos_ptr_t start)
{
    nvmos_dl_datalayerMeta_t *dlmeta = (nvmos_dl_datalayerMeta_t *)start;
    if (dlmeta->magic == NVMOS_DL_MAGIC)
    {
        return dlmeta;
    }
    return NULL;
}

int datalayer_createDatalayer(
    nvmos_ptr_t start,
    size_t size,
    size_t allocationBlockSize)
{
    if (start % allocationBlockSize != 0)
        return NULL;
    if (size % allocationBlockSize != 0)
        return NULL;

    nvmos_dl_datalayerMeta_t *dlmeta = (nvmos_dl_datalayerMeta_t *)start;
    dlmeta->startAddress = start;
    dlmeta->blockCount = size / allocationBlockSize;
    dlmeta->allocationBlockSize = allocationBlockSize;
    nvmos_dl_allocator_t allocator;
    if (nvmos_dl_alloc_createAllocator(
            &allocator,
            start + allocationBlockSize,
            dlmeta->blockCount - 1,
            allocationBlockSize))
    {
        return NULL;
    }
    dlmeta->freeBlockList = (nvmos_ptr_t)allocator.head;

    meta_metaBlk_t *firstMetaBlock = NULL;

    meta_meta_t *kroot = meta_getNextFreeMeta(&firstMetaBlock);
    if (kroot == NULL)
        return NULL;
    meta_setDir(kroot);
    kroot->metaId = 0;
    kroot->parentMetaPtr = NULL;
    kroot->metaContent.fileMeta.refCount = 0;
    kroot->metaContent.fileMeta.fileSize = 0;
    kroot->metaContent.fileMeta.blkSize = 0;
    kroot->metaContent.fileMeta._1stBlk = NULL;
    kroot->metaContent.fileMeta._1stPtrBlk = NULL;
    kroot->metaContent.fileMeta._2ndPtrBlk = NULL;
    kroot->metaContent.fileMeta._3rdPtrBlk = NULL;
    dlmeta->kroot = (nvmos_ptr_t)kroot;

    meta_meta_t *uroot = meta_getNextFreeMeta(&firstMetaBlock, &allocator);
    if (uroot == NULL)
        return NULL;
    meta_setDir(uroot);
    uroot->metaId = 0;
    uroot->parentMetaPtr = NULL;
    uroot->metaContent.fileMeta.refCount = 0;
    uroot->metaContent.fileMeta.fileSize = 0;
    uroot->metaContent.fileMeta.blkSize = 0;
    uroot->metaContent.fileMeta._1stBlk = NULL;
    uroot->metaContent.fileMeta._1stPtrBlk = NULL;
    uroot->metaContent.fileMeta._2ndPtrBlk = NULL;
    uroot->metaContent.fileMeta._3rdPtrBlk = NULL;
    dlmeta->uroot = (nvmos_ptr_t)uroot;

    dlmeta->metaBlockList = (nvmos_ptr_t)firstMetaBlock;

    dlmeta->magic = NVMOS_DL_MAGIC;
    return dlmeta;
}

size_t datalayer_getSizeOfDatalayer(
    nvmos_dl_datalayerMeta_t *dlmeta)
{
    return dlmeta->blockCount * dlmeta->allocationBlockSize;
}

size_t datalayer_getPageSizeOfDatalayer(
    nvmos_dl_datalayerMeta_t *dlmeta)
{
    return dlmeta->allocationBlockSize;
}

int datalayer_resizeDatalayer(
    nvmos_dl_datalayerMeta_t *dlmeta,
    size_t newSize)
{
    size_t oldSize = datalayer_getSizeOfDatalayer(dlmeta);
    if (newSize <= oldSize)
    {
        return -1;
    }
    else
    {
        size_t blockCount = newSize / dlmeta->allocationBlockSize;
        nvmos_ptr_t startOfNewMem = dlmeta->startAddress + oldSize;
        nvmos_dl_allocator_t allocator;
        if (datalayer_getAllocator(&allocator, dlmeta))
            return -1;
        if (nvmos_dl_alloc_deallocateBlocks(
                &allocator,
                startOfNewMem,
                blockCount))
            return -1;
    }
    return 0;
}

int datalayer_getAllocator(
    nvmos_dl_allocator_t *allocator,
    nvmos_dl_datalayerMeta_t *dlmeta)
{
    return nvmos_dl_alloc_retrieveAllocator(
        allocator,
        dlmeta->freeBlockList,
        dlmeta->allocationBlockSize);
}

meta_meta_t *datalayer_getURoot(
    nvmos_dl_datalayerMeta_t *dlmeta)
{
    return (meta_meta_t *)dlmeta->uroot;
}

meta_meta_t *datalayer_getKRoot(
    nvmos_dl_datalayerMeta_t *dlmeta)
{
    return (meta_meta_t *)dlmeta->kroot;
}