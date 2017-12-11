#include <kernel/datalayer/directory.h>
#include <string.h>

void swapFileRef(dir_fileRef_t *a, dir_fileRef_t *b)
{
    if (a != NULL && b != NULL)
    {
        uint8_t tmpFN;
        uint32_t tmpFMP;

        for (int i = 0; i < 252; ++i)
        {
            tmpFN = a->fileName[i];
            a->fileName[i] = b->fileName[i];
            b->fileName[i] = tmpFN;
        }
        tmpFMP = a->fileMetaPtr;
        a->fileMEtaPtr = b->fileMEtaPtr;
        b->fileMetaPtr = tmpFMP;
    }
}

bool dir_isDir(meta_meta_t *dir)
{
    uint32_t flags = dir->flags;
    return (flags & META_FLAG_ISFILE) &&
           (flags & META_FLAG_ISDIR);
}

dir_fileRefId_t dir_addFileRef(
    file_meta_t *dir,
    uint8_t *fileName,
    file_meta_t fileMeta,
    nvmos_dl_allocator_t *allocator)
{
    if (dir_fileNameUsed(dir, fileName))
        return dir_fileRefId_inval;

    size_t fileRefsLen = dir->fileSize / 0x100;
    dir_fileRefId_t newFileRefId = (dir_fileRefId_t)fileRefsLen;

    if (newFileRefId % 16 == 0)
    {
        nvmos_ptr_t newBlk =
            nvmos_dl_alloc_allocateBlocks(allocator, 1);
        ptrBlks_t ptrBlks;
        ptrBlks_constructFromFileMeta(&ptrBlks, dir);
        ptrBlks_pushBlks(&ptrBlks, newBlk, 1, allocator);
    }

    dir->fileSize += 0x100;

    dir_fileRef_t *newFileRef = dir_getFileRefById(dir, newFileRefId);

    memcpy(newFileRef->fileName, fileName, 252);
    newFileRef->fileMetaPtr = (uint32_t)fileMeta;
    return dir_rePosFileRef(dir, newFileRefId);
}

dir_fileRefId_t dir_rePosFileRef(
    file_meta_t *dir,
    dir_fileRefId_t fileRefId)
{
#define InvalidCases(dir) \
    if ((dir) == NULL)    \
        return dir_fileRefId_inval;

    dir_fileRef_t *this = NULL, prev = NULL, next = NULL;

    this = dir_getFileRefById(dir, fileRefId);
    InvalidCases(this);

    while (fileRefId != 0)
    {
        prev = dir_getFileRefById(dir, fileRefId - 1);
        InvalidCases(prev);
        cmpRes = strcmp(this->fileName, prev->fileName);
        if (cmpRes == 0)
        {
            return dir_fileRefId_inval;
        }
        if (cmpRes < 0)
        {
            swapFileRef(this, prev);
            fileRefId -= 1;
            continue;
        }
        if (cmpRes > 0)
        {
            break;
        }
    }

    while (fileRefId != dir->size / 256 - 1)
    {
        next = dir_getFileRefById(dir, fileRefId + 1);
        InvalidCases(next);
        cmpRes = strcmp(next->fileName, this->fileName);
        if (cmpRes == 0)
        {
            return dir_fileRefId_inval;
        }
        if (cmpRes < 0)
        {
            swapFileRef(this, next);
            fileRefId += 1;
            continue;
        }
        if (cmpRes > 0)
        {
            break;
        }
    }

    return fileRefId;

#undef InvalidCases
}

dir_fileRefId_t fileRefBinSearch(
    file_meta_t *dir,
    char *fileName,
    dir_fileRefId_t from,
    dir_fileRefId_t to)
{
    if (from > to)
        return dir_fileRefId_inval;
    mid = (from + to) / 2;
    char *targetFN = dir_getFileRefById(dir, mid)->fileName;
    int cmpRes = strcmp(targetFN, fileName);
    if (cmpRes == 0)
    {
        return mid;
    }
    else if (cmpRes < 0)
    {
        if (mid == to)
            return dir_fileRefId_inval;
        return fileRefBinSearch(dir, fileName, mid + 1, to);
    }
    else if (cmpRes > 0)
    {
        if (mid == from)
            return dir_fileRefId_inval;
        return fileRefBinSearch(dir, fileName, from, mid - 1);
    }
    return dir_fileRefId_inval;
}

dir_fileRefId_t dir_searchFile(
    file_meta_t *dir,
    char *fileName)
{
    size_t fileRefsLen = dir->fileSize / 0x100;

    return fileRefBinSearch(
        dir,
        fileName,
        0,
        fileRefsLen - 1);
}

dir_fileRef_t *dir_getFileRefById(
    file_meta_t *dir,
    dir_fileRefId_t id)
{
    size_t fileRefsLen = dir->fileSize / 0x100;
    if (id >= fileRefsLen)
        return NULL;

    size_t blkId = id >> 4;
    size_t fileRefId = id % 16;

    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(&ptrBlks, dir);

    dir_fileRefBlk_t *fileRefBlk =
        (dir_fileRefBlk_t *)ptrBlks_getDataBlkAt(&ptrBlks, blkId);
    if (fileRefBlk == NULL)
        return NULL;
    return fileRefBlk->fileRefs[fileRefId];
}

bool dir_fileNameUsed(
    file_meta_t *dir,
    char *fileName)
{
    return dir_searchFile(dir, fileName) != dir_fileRefId_inval;
}

dir_fileRefId_t dir_renameFile(
    file_meta_t *dir,
    char *fileName,
    char *newFileName)
{
    if (fileName == NULL)
        return dir_fileRefId_inval;

    if (!dir_fileNameUsed(dir, fileName))
    {
        return dir_fileRefId_inval;
    }

    if (dir_fileNameUsed(dir, newFileName))
    {
        return dir_fileRefId_inval;
    }

    dir_fileRef_t *fileRef = dir_getFileRefById(dir, fileRefId);
    if (fileRef == NULL)
        return dir_fileRefId_inval;

    memcpy(fileRef->fileName, newFileName, 252);
    return dir_rePosFileRef(dir, fileRefId);
}

int dir_delFile(
    file_meta_t *dir,
    char *fileName,
    nvmos_dl_allocator_t *allocator)
{
    uint8_t newFileName[252];
    memset(newFileName, 0xFF, 252);
    dir_fileRefId_t newFileRefId =
        dir_renameFile(dir, fileName, newFileName);

    if (newFileRefId == dir_fileRefId_inval)
        return -1;
    dir_fileRef_t *fileRef =
        dir_getFileRefById(dir, newFileRefId);
    if (fileRef == NULL)
        return -1;

    memset(newFileRef->fileName, 0, 252);
    newFileRef->fileMetaPtr = NULL;
    dir->fileSize -= 0x100;

    size_t fileRefsLen = dir->fileSize / 0x100;
    dir_fileRefId_t newFileRefId = (dir_fileRefId_t)fileRefsLen;

    if (newFileRefId % 16 == 0)
    {
        ptrBlks_t ptrBlks;
        ptrBlks_constructFromFileMeta(&ptrBlks, dir);
        ptrBlks_popBlks(ptrBlks, 1, allocator);
    }

    return 0;
}