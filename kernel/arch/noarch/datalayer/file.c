#include <kernel/datalayer/file.h>

int file_read(
    file_meta_t *meta,
    char *buf,
    size_t pos,
    size_t len)
{
}

int file_write(
    file_meta_t *meta,
    char *buf,
    size_t pos,
    size_t len)
{
}

int file_append(
    file_meta_t *meta,
    char *buf,
    size_t len)
{
}

int file_discardTail(
    file_meta_t *meta,
    size_t len)
{
}

int file_removeFile(
    file_meta_t *meta)
{
}

int file_getMap(
    file_meta_t *meta,
    nvmos_ptr_t *buf,
    size_t fromBlk,
    size_t len)
{
}