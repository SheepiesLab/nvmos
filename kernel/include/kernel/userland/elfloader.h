#include <kernel/datalayer/file.h>
#include <kernel/datalayer/proc.h>
#include <kernel/datalayer/allocator/allocator.h>

int elf_loadExec(
	file_meta_t *exec,
	proc_meta_t *proc,
	nvmos_dl_allocator_t *alloc);