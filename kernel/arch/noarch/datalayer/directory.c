#include <kernel/datalayer/directory.h>
#include <kernel/datalayer/ptrBlks.h>
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
        tmpFMP = a->metaPtr;
        a->metaPtr = b->metaPtr;
        b->metaPtr = tmpFMP;
    }
}

bool dir_isDir(meta_meta_t *dir)
{
    uint32_t flags = dir->flags;
    return (flags & META_FLAG_ISFILE) &&
           (flags & META_FLAG_ISDIR);
}

#define allocTest(num)                                              \
    printf("%d Allocator.head: 0x%p\n", num, allocator->head);      \
    for (int i = 0; i < testCount; ++i)                             \
    {                                                               \
        testAlloc[i] = nvmos_dl_alloc_allocateBlocks(allocator, 1); \
        printf("Test Alloc:     0x%p\n",                            \
               (uint64_t)testAlloc[i]);                             \
    }                                                               \
    printf("%d Allocator.head: 0x%p\n", num, allocator->head);

dir_fileRefId_t dir_addFileRef(
    file_meta_t *dir,
    uint8_t *fileName,
    meta_meta_t *fileMeta,
    nvmos_dl_allocator_t *allocator)
{

    uint32_t testCount = 3;
    nvmos_ptr_t testAlloc[testCount];
    allocTest(0);
    if (dir_fileNameUsed(dir, (char *)fileName))
        return dir_fileRefId_inval;
    allocTest(1);

    size_t fileRefsLen = dir->fileSize / 0x100;
    dir_fileRefId_t newFileRefId = (dir_fileRefId_t)fileRefsLen;
    allocTest(2);

    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(&ptrBlks, dir);

    if (newFileRefId % 16 == 0)
    {
        allocTest(3);
        nvmos_ptr_t newBlk =
            nvmos_dl_alloc_allocateBlocks(allocator, 1);
        if (newBlk == NULL)
        {
            printf("Cannot allocate memory for file ref...\n");
            return dir_fileRefId_inval;
        }
        memset((void *)newBlk, 0, 0x1000);
        allocTest(4);
        if (ptrBlks_pushBlks(&ptrBlks, newBlk, 1, allocator))
        {
            printf("ptrBlks push block failed...\n");
            return dir_fileRefId_inval;
        }
        ptrBlks_saveToFileMeta(&ptrBlks, dir);
    }

    allocTest(5);
    dir->fileSize += 0x100;

    dir_fileRef_t *newFileRef = dir_getFileRefById(dir, newFileRefId);

    allocTest(6);
    memcpy(newFileRef->fileName, fileName, 252);
    newFileRef->metaPtr = (uint32_t)fileMeta;
    allocTest(7);
    ptrBlks_saveToFileMeta(&ptrBlks, dir);
    allocTest(8);
    return dir_rePosFileRef(dir, newFileRefId);
}
#undef allocTest
dir_fileRefId_t dir_rePosFileRef(
    file_meta_t *dir,
    dir_fileRefId_t fileRefId)
{
#define InvalidCases(dir) \
    if ((dir) == NULL)    \
        return dir_fileRefId_inval;

    dir_fileRef_t *this = NULL;
    dir_fileRef_t *prev = NULL;
    dir_fileRef_t *next = NULL;

    this = dir_getFileRefById(dir, fileRefId);
    InvalidCases(this);

    while (fileRefId != 0)
    {
        prev = dir_getFileRefById(dir, fileRefId - 1);
        this = dir_getFileRefById(dir, fileRefId);
        InvalidCases(prev);
        int cmpRes = strcmp(this->fileName, prev->fileName);
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

    while (fileRefId != dir->fileSize / 256 - 1)
    {
        next = dir_getFileRefById(dir, fileRefId + 1);
        this = dir_getFileRefById(dir, fileRefId);
        InvalidCases(next);
        int cmpRes = strcmp(next->fileName, this->fileName);
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
    dir_fileRefId_t mid = (from + to) / 2;
    dir_fileRef_t *targetFileRef = dir_getFileRefById(dir, mid)->fileName;
    if (targetFileRef == NULL)
    {
        return dir_fileRefId_inval;
    }
    const char *targetFileName = targetFileRef->fileName;
    int cmpRes = strcmp(targetFileName, fileName);
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

dir_fileRefId_t dir_searchFileRef(
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
    return &(fileRefBlk->fileRefs[fileRefId]);
}

bool dir_fileNameUsed(
    file_meta_t *dir,
    char *fileName)
{
    return dir_searchFileRef(dir, fileName) != dir_fileRefId_inval;
}

dir_fileRefId_t dir_renameFileRef(
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

    dir_fileRefId_t fileRefId = dir_searchFileRef(dir, fileName);
    dir_fileRef_t *fileRef = dir_getFileRefById(dir, fileRefId);
    if (fileRef == NULL)
        return dir_fileRefId_inval;

    memcpy(fileRef->fileName, newFileName, 252);
    return dir_rePosFileRef(dir, fileRefId);
}

int dir_delFileRef(
    file_meta_t *dir,
    char *fileName,
    nvmos_dl_allocator_t *allocator)
{
    uint8_t newFileName[252];
    memset(newFileName, 0xFF, 252);
    dir_fileRefId_t newFileRefId =
        dir_renameFileRef(dir, fileName, newFileName);

    if (newFileRefId == dir_fileRefId_inval)
        return -1;
    dir_fileRef_t *fileRef =
        dir_getFileRefById(dir, newFileRefId);
    if (fileRef == NULL)
        return -1;

    memset(fileRef->fileName, 0, 252);
    fileRef->metaPtr = NULL;
    dir->fileSize -= 0x100;

    size_t fileRefsLen = dir->fileSize / 0x100;
    newFileRefId = (dir_fileRefId_t)fileRefsLen;

    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(&ptrBlks, dir);

    if (newFileRefId % 16 == 0)
    {

        ptrBlks_popBlks(&ptrBlks, 1, allocator);
    }

    ptrBlks_saveToFileMeta(
        &ptrBlks, dir);
    return 0;
}