#ifndef _FILE_H_
#define _FILE_H_

#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/ptrBlks.h>
#include <kernel/datalayer/allocator/allocator.h>

struct file_meta
{
    uint64_t refCount; //Fiel reference is the access point to file
    uint64_t fileSize;
    uint32_t _1stBlk;
    uint32_t _1stPtrBlk;
    uint32_t _2ndPtrBlk;
    uint32_t _3rdPtrBlk;
};
typedef struct file_meta file_meta_t;

#define FILE_MAXSIZE (PTRBLKS_MAXSIZE << 12)

size_t file_read(
    file_meta_t *file,
    char *buf,
    size_t pos,
    size_t len);

uint8_t file_getByteAt(
    file_meta_t *file,
    size_t pos);

size_t file_write(
    file_meta_t *filemeta,
    char *buf,
    size_t pos,
    size_t len,
    nvmos_dl_allocator_t *alloc);

size_t file_append(
    file_meta_t *file,
    char *buf,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int file_discardTail(
    file_meta_t *file,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int file_removeFile(
    file_meta_t *file,
    nvmos_dl_allocator_t *alloc);

size_t file_getMap(
    file_meta_t *file,
    nvmos_ptr_t *buf,
    size_t fromBlk,
    size_t len);

#endif