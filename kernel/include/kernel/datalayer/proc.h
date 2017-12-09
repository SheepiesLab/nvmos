
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