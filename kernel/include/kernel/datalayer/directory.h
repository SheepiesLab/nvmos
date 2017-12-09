#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/meta.h>

struct fileRef
{
	uint8_t fileName[252];
	uint32_t fileMetaPtr;
};
typedef struct fileRef dir_fileRef_t;

struct fileRefBlock {
	dir_fileRef_t fileRefs[16];
}
typedef struct fileRefBlock dir_fileRefBlock_t;

int dir_sortFileRefs(dir_fileRef_t *fileRefs, size_t length);

struct NVMOS_DL_fileMeta *dir_getFileMeta(dir_fileRef_t *fileRef);
struct NVMOS_DL_fileMeta *dir_searchFile(
	char *fileName,
	dir_fileRef_t *fileRef,
	size_t length);

int dir_delFile(dir_fileRef_t *fileRef);
