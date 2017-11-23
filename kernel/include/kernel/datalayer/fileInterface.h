#include "datalayer.h"

struct datalayer_fileHandle_struct;
typedefe struct datalayer_fileHandle_struct file_t;

char *fileInterface_listDirectoryContent (datalayer *datalayer, const char *target);

int *fileInterface_createDirectory (datalayer *datalayer, const char *target);

int fileInterface_createFile (datalayer_t *datalayer, const char *target);

file_t *fileInterface_openFile (datalayer_t *datalayer, const char *target, const char *mode);

int fileInterface_moveFileOrDirectory (datalayer_t *datalayer, const char *targetDirectory, const char *source);

int fileInterface_renameFileOrDirectory (datalayer_t *datalayer, const char *newName, const char *source);

int fileInterface_isFile (datalayer_t *datalayer, const char *target);

int fileInterface_isDirectory (datalayer_t *datalayer, const char *target);

int fileInterface_closeFile (file_t *file);

int fileInterface_deleteFile (datalayer_t *datalayer, char *path);

int fileInterface_readFile (file_t *file, char *buffer, size_t length);

int fileInterface_rewindFile (file_t *file, size_t positionFromStart);

int fileInterface_writeFile (file_t *file, const char* source, size_t length);

int fileInterface_mapFileToProcess (file_t *file, process_t *process, uptr_t position);

size_t fileInterface_getFileSize (file_t *file);

int fileInterface_resizeFile (file_t *file, size_t newSize);
