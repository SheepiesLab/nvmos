#define NVMOS_DL_FILENAMESIZE 2048

struct datalayer_fileReference_struct{
  uint64_t metaId;
  uint64_t timeCreated;
  uint64_t timeLastModified;
  uint64_t timeLastOpened;
  uint8_t name[NVMOS_DL_FILENAMESIZE];
}
