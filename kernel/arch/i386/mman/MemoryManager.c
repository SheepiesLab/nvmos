//
// Created by benng on 7/17/17.
//

#include <kernel/mman/MemoryManager.h>
#include <kernel/mman/KernelSection.h>
#include <kernel/mman/gdt.h>

int mman_construct(MemoryManager *mman, multiboot_info_t *mbt)
{
	mman->mbt = mbt;
	KernelSection *ksects = ksection_getKsections();

	GlobalDescriptor gdt[GDT_LEN];
	gd_fillEntry(&gdt[0], 0, 0, 0);
	gd_fillEntry(&gdt[1], 0, 0xffffffff, 0x9A);
	gd_fillEntry(&gdt[2], 0, 0xffffffff, 0x92);
	gd_fillEntry(
		&gdt[3],
		(uint32_t)ksects[KSECTION_SECTION_TSS].addr,
		(uint32_t)ksects[KSECTION_SECTION_TSS].len,
		0x89
		);

	gdt_commit(
		mman->gdtBuffer,
		gdt,
		GDT_LEN);

	heap_construct(
		&(mman->heap),
		ksects[KSECTION_SECTION_HEAP].addr,
		ksects[KSECTION_SECTION_HEAP].len +
		ksects[KSECTION_SECTION_HEAP].addr);

	return 0;
}

size_t mman_getMemoryMapLength(MemoryManager *mman)
{
	if ((mman->mbt->flags & MULTIBOOT_INFO_MEM_MAP) !=
	    MULTIBOOT_INFO_MEM_MAP)
		return -1;

	multiboot_memory_map_t *mmap =
		(multiboot_memory_map_t *)mman->mbt->mmap_addr;

	size_t i = 0;
	while (
		(nvmos_ptr_t)mmap <
		(mman->mbt->mmap_addr + mman->mbt->mmap_length)
		) {
		++i;
		mmap = (multiboot_memory_map_t *)
		       ((unsigned int)mmap + mmap->size + sizeof(mmap->size));
	}

	return i;
}

int mman_getMemoryMap(
	MemoryManager * mman,
	MemoryMap *		_mmap,
	size_t			max)
{
	if ((mman->mbt->flags & MULTIBOOT_INFO_MEM_MAP) !=
	    MULTIBOOT_INFO_MEM_MAP)
		return -1;

	multiboot_memory_map_t *mmap =
		(multiboot_memory_map_t *)mman->mbt->mmap_addr;

	size_t i = 0;
	while ((nvmos_ptr_t)mmap < (mman->mbt->mmap_addr + mman->mbt->mmap_length) &&
	       i < max
	       ) {
		_mmap[i].len = mmap->len;
		_mmap[i].addr = mmap->addr;
		_mmap[i].type = mmap->type;
		++i;
		mmap = (multiboot_memory_map_t *)
		       ((unsigned int)mmap + mmap->size + sizeof(mmap->size));
	}

	return i;
}
