#ifndef _PROC_H_
#define _PROC_H_

#include <kernel/datalayer/file.h>
#include <kernel/datalayer/pageDir.h>
#include <kernel/datalayer/allocator/allocator.h>

struct proc_meta
{
    uint32_t pageDir;
};

typedef struct proc_meta proc_meta_t;

int proc_createProc(
    proc_meta_t *meta,
    nvmos_dl_allocator_t *alloc);

int proc_allocBlks(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int proc_freeBlks(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int proc_mapFile(
    proc_meta_t *meta,
    file_meta_t *file,
    size_t blkPos,
    size_t len,
    uint8_t rwx,
    nvmos_ptr_t procAddr,
    nvmos_dl_allocator_t *alloc);

int proc_unmapFile(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int proc_mapKernel(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    nvmos_ptr_t kAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int proc_unmapKernel(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    nvmos_ptr_t kAddr,
    size_t len,
    nvmos_dl_allocator_t *alloc);

int proc_removeProc(
    proc_meta_t *meta,
    nvmos_dl_allocator_t *alloc);

#endif