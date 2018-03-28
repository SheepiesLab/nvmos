#include <kernel/datalayer/file.h>
#include <kernel/datalayer/proc.h>
#include <kernel/datalayer/datalayer.h>
#include <kernel/datalayer/allocator/allocator.h>

#include <stdint.h>

/**
 * @brief
 *
 * @param exec
 * @param proc
 * @param alloc
 * @return size_t Number of blocks mapped
 */
size_t elf_loadExec(
	file_meta_t *exec,
	proc_meta_t *proc,
	nvmos_dl_allocator_t *alloc);

int elf_exec(
	file_meta_t *exec,
	proc_meta_t *proc,
	nvmos_dl_allocator_t *alloc,
	nvmos_dl_datalayerMeta_t *dlmeta);