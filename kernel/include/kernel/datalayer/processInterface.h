#include "datalayer.h"

struct datalayer_processHandle_struct;
typedefe struct datalayer_processHandle_struct process_t;

process_t *datalayer_createProcess (datalayer_t *datalayer);

int datalayer_loadProgramToProcess (process_t *process, const char *path);

int datalayer_loadLibraryToProcess (process_t *process, const char *path);

int datalayer_getPageTableOfProcess (process_t *process, char **target);

int datalayer_allocatePageToProcess (process_t *process, size_t size, nvmos_ptr_t *allocationAddress);

int datalayer_freePageFromProcess (process_t *process, nvmos_ptr_t allocationAddress);

int datalayer_removeProcess (process_t *process);
