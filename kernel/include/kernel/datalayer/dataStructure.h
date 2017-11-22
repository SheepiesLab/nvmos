#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/allocator.h>
#include <kernel/kdef.h>

#define DATALAYER_BLOCK_SIZE 0x1000

struct datalayer_datalayerMeta_struct;
typedef struct datalayer_datalayerMeta_struct datalayer_datalayerMeta_t;

struct datalayer_meta_struct;
typedef struct datalayer_meta_struct datalayer_meta_t;

struct datalayer_fileMeta_struct;
typedef struct datalayer_fileMeta_struct datalayer_fileMeta_t;

struct datalayer_processMeta_struct;
typedef struct datalayer_processMeta_struct datalayer_processMeta_t;

struct datalayer_freeMeta_struct;
typedef struct datalayer_freeMeta_struct datalayer_freeMeta_t;


struct datalayer_directoryHashTable_struct;
typedef struct datalayer_directoryHashTable_struct datalayer_directoryHashTable_t;

struct datalayer_datalayerMeta_struct{
  kptr_t startAddress;
  size_t blockCount;
  size_t blockSize;
  datalayer_fileMeta_t *kroot;
  datalayer_fileMeta_t *uroot;
  datalayer_freeBlockListNode_t *freeBlockList;
  int locked;
  uint8_t osReserved[]; // Pad to block aligned.
  datalayer_metaBlock_t metaBlocks[]; // Determines maximum file/directory/process count.
};

struct datalayer_meta_struct{
  int isFileMeta;
  int isFree;
  union {
    struct datalayer_fileMeta_struct;
    struct datalayer_processMeta_struct;
    struct datalayer_freeMeta_struct;
  } metaContent;
};

struct datalayer_fileMeta_struct{
  char name[1024];
  kptr_t parent;
  int isDirectory;
  datalayer_fileMeta_t *linkOrigin;
  kptr_t firstBlock;
  kptr_t secondBlockPointers;
  kptr_t thirdBlockPointers;
  uint8_t osReserved[];
};

struct datalayer_processMeta_struct{
  uint32_t processId;
  datalayer_processMeta_t *parent;
  uint8_t pageTable[];
  kptr_t firstBlock;
  kptr_t secondBlockPointers;
  kptr_t thirdBlockPointers;
  uint8_t osReserved[];
};

struct datalayer_freeMeta_struct {
  datalayer_freeMeta_struct *next;
};



struct datalayer_directoryHashTable_struct{
  uint8_t key[8];
  datalayer_meta_t *metaPtr;
};
