#include <kernel/datalayer/file.h>
#include <kernel/datalayer/ptrBlks.h>
#include <string.h>

size_t file_read(
    file_meta_t *file,
    char *buf,
    size_t pos,
    size_t len)
{
    if (pos + len > file->fileSize)
    {
        len = file->fileSize - pos;
    }

    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(
        &ptrBlks, file);

    size_t bytesRead = 0;
#define bufPos bytesRead

    while (len > 0)
    {
        size_t inBlkPos = pos % 0x1000;
        size_t blkPos = pos >> 12;
        size_t bytesToBeCopied = 0x1000 - inBlkPos;
        if (len < bytesToBeCopied)
            bytesToBeCopied = len;

        ptrBlks_dataBlk_t *blkAddr =
            ptrBlks_getDataBlkAt(&ptrBlks, blkPos);
        if (blkAddr == NULL)
            return bytesRead;
        memcpy(
            &(buf[bufPos]),
            &(blkAddr->dataBuff[inBlkPos]),
            bytesToBeCopied);

        bytesRead += bytesToBeCopied;
        len -= bytesToBeCopied;
        pos += bytesToBeCopied;
    }

#undef bufPos

    return bytesRead;
}

size_t file_write(
    file_meta_t *file,
    char *buf,
    size_t pos,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    if (pos + len > file->fileSize)
    {
        ;
    }

    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(
        &ptrBlks, file);

    size_t bytesWritten = 0;
#define bufPos bytesWritten

    size_t newSize = pos + len;
    size_t blkLen = newSize >> 12;
    if (blkLen > ptrBlks_getSize(&ptrBlks))
    {
        size_t blksNeeded = blkLen - ptrBlks_getSize(&ptrBlks);
        nvmos_ptr_t newBlks =
            nvmos_dl_alloc_allocateBlocks(alloc, blksNeeded);
        if (ptrBlks_pushBlks(&ptrBlks, newBlks, newBlks, alloc))
        {
            ptrBlks_saveToFileMeta(
                &ptrBlks, file);
            return 0;
        }
    }

    while (len > 0)
    {
        size_t inBlkPos = pos % 0x1000;
        size_t blkPos = pos >> 12;
        size_t bytesToBeCopied = 0x1000 - inBlkPos;
        if (len < bytesToBeCopied)
            bytesToBeCopied = len;

        ptrBlks_dataBlk_t *blkAddr =
            ptrBlks_getDataBlkAt(&ptrBlks, blkPos);
        if (blkAddr == NULL)
        {
        }
        memcpy(
            &(blkAddr->dataBuff[inBlkPos]),
            &(buf[bufPos]),
            bytesToBeCopied);

        bytesWritten += bytesToBeCopied;
        len -= bytesToBeCopied;
        pos += bytesToBeCopied;
        file->fileSize += bytesToBeCopied;
    }

#undef bufPos
    ptrBlks_saveToFileMeta(
        &ptrBlks, file);
    return bytesWritten;
}

size_t file_append(
    file_meta_t *file,
    char *buf,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    return file_write(file, buf, file->fileSize, len, alloc);
}

int file_discardTail(
    file_meta_t *file,
    size_t len,
    nvmos_dl_allocator_t *alloc)
{
    if (len > file->fileSize)
    {
        return -1;
    }

    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(
        &ptrBlks, file);

    size_t newFileSize = file->fileSize - len;
    size_t oldBlkLen = file->fileSize >> 12;
    size_t newBlkLen = file->fileSize >> 12;
    file->fileSize = newFileSize;
    size_t blksToBePoped = oldBlkLen - newBlkLen;
    if (blksToBePoped > 0)
    {
        ptrBlks_popBlks(&ptrBlks, blksToBePoped, alloc);
    }
    ptrBlks_saveToFileMeta(
        &ptrBlks, file);
    return 0;
}

int file_removeFile(
    file_meta_t *file,
    nvmos_dl_allocator_t *alloc)
{
    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(
        &ptrBlks, file);

    size_t blkLen = ptrBlks_getSize(&ptrBlks);
    if (blkLen != 0)
    {
        ptrBlks_popBlks(&ptrBlks, blkLen, alloc);
    }
    return 0;
}

size_t file_getMap(
    file_meta_t *file,
    nvmos_ptr_t *buf,
    size_t fromBlk,
    size_t len)
{
    ptrBlks_t ptrBlks;
    ptrBlks_constructFromFileMeta(
        &ptrBlks, file);

    for (size_t i = 0; i < len; ++i)
    {
        buf[i] =
            (nvmos_ptr_t)ptrBlks_getDataBlkAt(&ptrBlks, i);
        if (buf[i] == NULL)
            return i;
    }
    return len;
}