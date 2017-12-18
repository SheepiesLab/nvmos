#include <kernel/datalayer/meta.h>

meta_meta_t *meta_getNextFreeMeta(
    meta_metaBlk_t **metaBlk,
    nvmos_dl_allocator_t *alloc)
{
    meta_metaBlk_t *currentMetaBlk = *metaBlk;
    meta_metaBlk_t *lastMetaBlk = NULL;
    while (currentMetaBlk != NULL)
    {
        for (size_t i = 0; i < 93; ++i)
        {
            meta_meta_t *nextMeta =
                &(currentMetaBlk->metas[i]);
            if (meta_isFree(nextMeta))
                return nextMeta;
        }
        lastMetaBlk = currentMetaBlk;
        currentMetaBlk =
            (meta_metaBlk_t *)currentMetaBlk->next;
    }
    nvmos_ptr_t newBlk =
        nvmos_dl_alloc_allocateBlocks(alloc, 1);
    if (newBlk == NULL)
        return NULL;

    if (lastMetaBlk == NULL)
    {
        *metaBlk = (meta_metaBlk_t *)newBlk;
        return &((*metaBlk)->metas[0]);
    }

    lastMetaBlk->next = (uint32_t)newBlk;
    currentMetaBlk =
            (meta_metaBlk_t *)lastMetaBlk->next;
    return &(currentMetaBlk->metas[0]);
}

bool meta_isFile(meta_meta_t *meta)
{
    return (meta->flags & META_FLAG_ISFILE &&
            !(meta->flags & META_FLAG_ISDIR) &&
            !(meta->flags & META_FLAG_FREE));
}
bool meta_isDir(meta_meta_t *meta)
{
    return (meta->flags & META_FLAG_ISFILE &&
            meta->flags & META_FLAG_ISDIR &&
            !(meta->flags & META_FLAG_FREE));
}
bool meta_isProc(meta_meta_t *meta)
{
    return (!(meta->flags & META_FLAG_ISFILE) &&
            !(meta->flags & META_FLAG_ISDIR) &&
            !(meta->flags & META_FLAG_FREE));
}
bool meta_isFree(meta_meta_t *meta)
{
    return meta->flags & META_FLAG_FREE;
}

void meta_setFile(meta_meta_t *meta)
{
    meta->flags |= META_FLAG_ISFILE;
    meta->flags &= ~META_FLAG_ISDIR;
    meta->flags &= ~META_FLAG_FREE;
}
void meta_setDir(meta_meta_t *meta)
{
    meta->flags |= META_FLAG_ISFILE;
    meta->flags |= META_FLAG_ISDIR;
    meta->flags &= ~META_FLAG_FREE;
}
void meta_setProc(meta_meta_t *meta)
{
    meta->flags &= ~META_FLAG_ISFILE;
    meta->flags &= ~META_FLAG_ISDIR;
    meta->flags &= ~META_FLAG_FREE;
}
void meta_setFree(meta_meta_t *meta)
{
    meta->flags &= ~META_FLAG_ISFILE;
    meta->flags &= ~META_FLAG_ISDIR;
    meta->flags |= META_FLAG_FREE;
}