#include <kernel/userland/elfloader.h>
#include <kernel/userland/elf.h>

#define MAX_FILE_BLKSIZE 0x400

// Implementation assume file is in consecutive blocks
// If it is not the case, I need to build virtual address
// space for kernel first, which I have no plan to do.
//
// I don't want to copy it to kernel memory either, as
// it defeats the purpose of execution in-place
int elf_loadExec(
	file_meta_t *exec,
	proc_meta_t *proc,
	nvmos_dl_allocator_t *alloc)
{
	nvmos_ptr_t execBlks[MAX_FILE_BLKSIZE];
	size_t execBlkSize;

	execBlkSize = file_getMap(exec, execBlks, 0, MAX_FILE_BLKSIZE);

	Elf32_Ehdr *elfHeader = (Elf32_Ehdr *)execBlks[0];
	if (!elf_check_supported(elfHeader))
	{
		return -1;
	}

	if (elfHeader->e_type != ET_EXEC)
	{
		ERROR("ELF File cannot be executed.\n");
		return -1;
	}

	nvmos_ptr_t programHeaders =
		(uint32_t)elfHeader + elfHeader->e_phoff;
}