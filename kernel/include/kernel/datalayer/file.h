#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/ptrBlks.h>

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

int file_read(
    file_meta_t *meta,
    char *buf,
    size_t pos,
    size_t len);

int file_write(
    file_meta_t *meta,
    char *buf,
    size_t pos,
    size_t len);

int file_append(
    file_meta_t *meta,
    char *buf,
    size_t len);

int file_discardTail(
    file_meta_t *meta,
    size_t len);

int file_removeFile(
    file_meta_t *meta);

int file_getMap(
    file_meta_t *meta,
    nvmos_ptr_t *buf,
    size_t fromBlk,
    size_t len);