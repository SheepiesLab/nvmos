
#include <kernel/datalayer/datalayer.h>

struct proc_meta
{
    uint32_t procId;
    uint32_t parentId;
    uint64_t pageDir;
    uint64_t openedFiles;
    uint64_t procSize;
    uint64_t _1stBlk;
    uint64_t _1stPtrBlk;
    uint64_t _2ndPtrBlk;
    uint64_t _3rdPtrBlk;
};

typedef struct proc_meta proc_meta_t;

int proc_allocBlks(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len);

int proc_freeBlks(
    proc_meta_t *meta,
    nvmos_ptr_t procAddr,
    size_t len);

int proc_openFile(
    proc_meta_t *meta,
    file_meta_t *file);

int proc_closeFile(
    proc_meta_t *meta,
    file_meta_t *file);

int proc_mapFile(
    proc_meta_t *meta,
    file_meta_t *file,
    size_t blkPos,
    size_t len,
    nvmos_ptr_t procAddr);

int proc_removeProc(
    proc_meta_t *meta);