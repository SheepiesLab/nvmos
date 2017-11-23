struct datalayer_datalayerHandle_struct;
typedefe struct datalayer_datalayerHandle_struct datalayer_t;

#define NVMOS_DL_DLMETA_FLAG_LOCKED 0x1

typedef struct {
	uint64_t	startAddress;
	uint64_t	blockCount;
	uint64_t	allocationBlockSize;
	uint64_t	kroot;
	uint64_t	uroot;
	uint64_t	freeBlockList;
	uint64_t	metaBlockList;
	uint32_t	flags;
} datalayer_datalayerMeta_t;

datalayer_t *datalayer_loadDatalayer(nvmos_ptr_t start);

datalayer_t *datalayer_createDatalayer(nvmos_ptr_t start, size_t size, size_t pageSize);

size_t datalayer_getSizeOfDatalayer(datalayer_t *datalayer);

size_t datalayer_getPageSizeOfDatalayer(datalayer_t *datalayer);

int datalayer_resizeDatalayer(datalayer_t *datalayer, size_t newSize);

int datalayer_unloadDatalayer(datalayer_t *datalayer);
