
#ifndef NVMOS_DATALAYER_H_
#define NVMOS_DATALAYER_H_

#include <stdint.h>
#include <kernel/kdef.h>

#define BLOCK_SIZE 0x1000

struct nvmos_dl_datalayerHandle_struct;
typedef struct nvmos_dl_datalayerHandle_struct datalayer_t;

// #define NVMOS_DL_DLMETA_FLAG_LOCKED 0x1

typedef struct
{
	uint64_t startAddress;
	uint64_t blockCount;
	uint64_t allocationBlockSize;
	uint64_t kroot;
	uint64_t uroot;
	uint64_t freeBlockList;
	uint64_t metaBlockList;
	uint32_t flags;
} nvmos_dl_datalayerMeta_t;

int datalayer_loadDatalayer(
	datalayer_t *datalayer,
	nvmos_ptr_t start);

int datalayer_createDatalayer(
	datalayer_t *datalayer,
	nvmos_ptr_t start,
	size_t size,
	size_t allocationBlockSize);

size_t datalayer_getSizeOfDatalayer(
	datalayer_t *datalayer);

size_t datalayer_getPageSizeOfDatalayer(
	datalayer_t *datalayer);

int datalayer_resizeDatalayer(
	datalayer_t *datalayer,
	size_t newSize);

int datalayer_unloadDatalayer(
	datalayer_t *datalayer);



struct nvmos_dl_fileHandle_struct;
typedefe struct nvmos_dl_fileHandle_struct file_t;

char *nvmos_dl_listDirectoryContent (datalayer *datalayer, const char *target);

int *nvmos_dl_createDirectory (datalayer *datalayer, const char *target);

int nvmos_dl_createFile (datalayer_t *datalayer, const char *target);

file_t *nvmos_dl_openFile (datalayer_t *datalayer, const char *target);

int nvmos_dl_moveFileOrDirectory (datalayer_t *datalayer, const char *targetDirectory, const char *source);

int nvmos_dl_renameFileOrDirectory (datalayer_t *datalayer, const char *newName, const char *source);

int nvmos_dl_isFile (datalayer_t *datalayer, const char *target);

int nvmos_dl_isDirectory (datalayer_t *datalayer, const char *target);

int nvmos_dl_closeFile (file_t *file);

int nvmos_dl_deleteFile (datalayer_t *datalayer, char *path);

int nvmos_dl_readFile (file_t *file, char *buffer, size_t length);

int nvmos_dl_rewindFile (file_t *file, size_t positionFromStart);

int nvmos_dl_writeFile (file_t *file, const char* source, size_t length);

int nvmos_dl_mapFileToProcess (file_t *file, process_t *process, uptr_t position);

size_t nvmos_dl_getFileSize (file_t *file);

int nvmos_dl_resizeFile (file_t *file, size_t newSize);



struct nvmos_dl_processHandle_struct;
typedefe struct nvmos_dl_processHandle_struct process_t;

process_t *nvmos_dl_createProcess (datalayer_t *datalayer);

int nvmos_dl_loadProgramToProcess (process_t *process, const char *path);

int nvmos_dl_loadLibraryToProcess (process_t *process, const char *path);

int nvmos_dl_getPageTableOfProcess (process_t *process, char **target);

int nvmos_dl_allocatePageToProcess (process_t *process, size_t size, nvmos_ptr_t *allocationAddress);

int nvmos_dl_freePageFromProcess (process_t *process, nvmos_ptr_t allocationAddress);

int nvmos_dl_removeProcess (process_t *process);

#endif