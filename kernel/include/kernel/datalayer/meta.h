#define NVMOS_DL_META_FLAG_ISFILE 0x1 // Either file meta or process meta
#define NVMOS_DL_META_FLAG_ISDIR  0x2
#define NVMOS_DL_META_FLAG_FREE   0x4

struct NVMOS_DL_fileMeta{
  uint64_t refCount; //Fiel reference is the access point to file
  uint64_t contentLength;
  uint64_t firstBlock;
  uint64_t secondBlockPointers;
  uint64_t thirdBlockPointers;
};

struct NVMOS_DL_processMeta{
  uint32_t processId;
  uint32_t parentId;
  uint64_t pageTableLocation;
  uint64_t pageTableLength;
  uint64_t contentLength;
  uint64_t firstBlock;
  uint64_t secondBlockPointers;
  uint64_t thirdBlockPointers;
};

struct NVMSO_DL_freeMeta {
  uint64_t nextFreeMeta;
};

struct NVMOS_DL_meta{
  uint64_t metaId;
  union{
    datalayer_fileMeta_t fileMeta;
    datalayer_processMeta_t processMeta;
    datalayer_freeMeta_t freeMeta;
  } metaContent;
  uint32_t flags;
};
