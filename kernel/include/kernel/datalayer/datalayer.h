struct datalayer_datalayerHandle_struct;
typedefe struct datalayer_datalayerHandle_struct datalayer_t;

datalayer_t *datalayer_loadDatalayer (kptr_t start);

datalayer_t *datalayer_createDatalayer (kptr_t start, size_t size, size_t pageSize);

size_t datalayer_getSizeOfDatalayer (datalayer_t *datalayer);

size_t datalayer_getPageSizeOfDatalayer (datalayer_t *datalayer);

int datalayer_resizeDatalayer (datalayer_t *datalayer, size_t newSize);

int datalayer_unloadDatalayer (datalayer_t *datalayer);
