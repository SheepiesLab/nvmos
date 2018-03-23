#include <kernel/datalayer/proc.h>
#include <kernel/datalayer/ptrBlks.h>

/*
	Implementatioin Sidenote

	The page directory structure has two layers: Page directory and page table.

	The page directory is a 4KB structure, containing 1024 entries. Each entry has a 
	physical address to either the page table or the page (big page enabled). Each 
	entry also has flags controlling the access to those tables or the page. The 
	flags of page table entries should be set to kernel read/write present, regardless 
	of the page access privilege. This is to make sure that no user process can modify 
	the memory mapping and tamper with the restricted access part of the memory.
*/

int proc_createProc(
	proc_meta_t *meta,
	nvmos_dl_allocator_t *alloc)
{
	meta->pageDir =
		(uint32_t)nvmos_dl_alloc_allocateBlocks(alloc, 1);
	if (meta->pageDir == NULL)
		return -1;
	memset(meta->pageDir, 0, 0x1000);
	return 0;
}

int proc_allocBlks(
	proc_meta_t *meta,
	nvmos_ptr_t procAddr,
	size_t len,
	nvmos_dl_allocator_t *alloc)
{
	pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
	uint32_t pageDirFlags =
		PAGEDIR_PRESENT |
		PAGEDIR_RW;
	uint32_t pageTableFlags =
		PAGETABLE_PRESENT |
		PAGETABLE_RW |
		PAGETABLE_USER;
	procAddr =
		(procAddr % 0x1000 == 0) ? procAddr : (procAddr & 0xFFFFF000) + 0x1000;
	nvmos_ptr_t blocks[len];
	blocks[0] = nvmos_dl_alloc_allocateBlocks(alloc, len);
	for (size_t i = 1; i < len; ++i)
	{
		blocks[i] = blocks[i - 1] + 0x1000;
	}
	return pageDir_mapSegment(
		pageDir,
		procAddr,
		len,
		blocks,
		alloc,
		pageDirFlags,
		pageTableFlags);
}

int proc_freeBlks(
	proc_meta_t *meta,
	nvmos_ptr_t procAddr,
	size_t len,
	nvmos_dl_allocator_t *alloc)
{
	pageDir_t *pageDir = (pageDir_t *)meta->pageDir;

	nvmos_ptr_t current = procAddr & 0xFFFFF000;

	for (size_t i = 0; i < len; ++i)
	{
		size_t pageTbIdx, pageIdx;
		pageDir_entryIdxOf(current, &pageTbIdx, &pageIdx);

		if (pageDir_isPageTableFlagSet(
				pageDir->page_tbs[pageTbIdx],
				PAGEDIR_PRESENT))
		{
			pageTable_t *pageTable = pageDir_ptrOfEntry(
				pageDir->page_tbs[pageTbIdx]);
			uint32_t *pageEntry = &(pageTable->pages[pageIdx]);
			if (pageDir_isPageFlagSet(*pageEntry, PAGETABLE_PRESENT) &&
				!pageDir_isPageFlagSet(*pageEntry, PAGETABLE_FILEMAPPED) &&
				!pageDir_isPageFlagSet(*pageEntry, PAGETABLE_KERNEL))
			{
				nvmos_ptr_t blockToBeFreed = pageDir_addressOfEntry(*pageEntry);
				*pageEntry = 0;
				nvmos_dl_alloc_deallocateBlocks(alloc, blockToBeFreed, 1);
			}
		}
		current += 0x1000;
	}

	return 0;
}

int proc_mapFile(
	proc_meta_t *meta,
	file_meta_t *file,
	size_t blkPos,
	size_t len,
	uint8_t rwx,
	nvmos_ptr_t procAddr,
	nvmos_dl_allocator_t *alloc)
{
	pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
	uint32_t pageDirFlags =
		PAGEDIR_PRESENT |
		PAGEDIR_RW;
	uint32_t pageTableFlags =
		PAGETABLE_PRESENT |
		PAGETABLE_FILEMAPPED;

	if (rwx & PROC_R)
		pageTableFlags |= PAGETABLE_USER;

	if (rwx & PROC_W)
		pageTableFlags |= PAGETABLE_RW;

	procAddr =
		(procAddr % 0x1000 == 0) ? procAddr : (procAddr & 0xFFFFF000) + 0x1000;
	nvmos_ptr_t blocks[len];

	size_t mappedSize = file_getMap(file, blocks, blkPos, len);

	return pageDir_mapSegment(
		pageDir,
		procAddr,
		mappedSize,
		blocks,
		alloc,
		pageDirFlags,
		pageTableFlags);
}

int proc_unmapFile(
	proc_meta_t *meta,
	nvmos_ptr_t procAddr,
	size_t len,
	nvmos_dl_allocator_t *alloc)
{
	pageDir_t *pageDir = (pageDir_t *)meta->pageDir;

	nvmos_ptr_t current = procAddr & 0xFFFFF000;

	for (size_t i = 0; i < len; ++i)
	{
		size_t pageTbIdx, pageIdx;
		pageDir_entryIdxOf(current, &pageTbIdx, &pageIdx);

		if (pageDir_isPageTableFlagSet(
				pageDir->page_tbs[pageTbIdx],
				PAGEDIR_PRESENT))
		{
			pageTable_t *pageTable = pageDir_ptrOfEntry(
				pageDir->page_tbs[pageTbIdx]);
			uint32_t *pageEntry = &(pageTable->pages[pageIdx]);
			if (pageDir_isPageFlagSet(*pageEntry, PAGETABLE_PRESENT) &&
				pageDir_isPageFlagSet(*pageEntry, PAGETABLE_FILEMAPPED) &&
				!pageDir_isPageFlagSet(*pageEntry, PAGETABLE_KERNEL))
			{
				*pageEntry = 0;
			}
		}
		current += 0x1000;
	}

	return 0;
}

int proc_mapKernel(
	proc_meta_t *meta,
	nvmos_ptr_t procAddr,
	nvmos_ptr_t kAddr,
	size_t len,
	nvmos_dl_allocator_t *alloc)
{
	pageDir_t *pageDir = (pageDir_t *)meta->pageDir;
	uint32_t pageDirFlags =
		PAGEDIR_PRESENT |
		PAGEDIR_USER | ////TODO test syscall only
		PAGEDIR_RW;
	uint32_t pageTableFlags =
		PAGETABLE_PRESENT |
		PAGETABLE_USER | //TODO test syscall only
		PAGETABLE_RW |
		PAGETABLE_KERNEL;
	procAddr =
		(procAddr % 0x1000 == 0) ? procAddr : (procAddr & 0xFFFFF000) + 0x1000;
	nvmos_ptr_t blocks[len];
	blocks[0] = kAddr;
	for (size_t i = 1; i < len; ++i)
	{
		blocks[i] = blocks[i - 1] + 0x1000;
	}
	return pageDir_mapSegment(
		pageDir,
		procAddr,
		len,
		blocks,
		alloc,
		pageDirFlags,
		pageTableFlags);
}

int proc_unmapKernel(
	proc_meta_t *meta,
	nvmos_ptr_t procAddr,
	nvmos_ptr_t kAddr,
	size_t len,
	nvmos_dl_allocator_t *alloc)
{
	pageDir_t *pageDir = (pageDir_t *)meta->pageDir;

	nvmos_ptr_t current = procAddr & 0xFFFFF000;

	for (size_t i = 0; i < len; ++i)
	{
		size_t pageTbIdx, pageIdx;
		pageDir_entryIdxOf(current, &pageTbIdx, &pageIdx);

		if (pageDir_isPageTableFlagSet(
				pageDir->page_tbs[pageTbIdx],
				PAGEDIR_PRESENT))
		{
			pageTable_t *pageTable = pageDir_ptrOfEntry(
				pageDir->page_tbs[pageTbIdx]);
			uint32_t *pageEntry = &(pageTable->pages[pageIdx]);
			if (pageDir_isPageFlagSet(*pageEntry, PAGETABLE_PRESENT) &&
				!pageDir_isPageFlagSet(*pageEntry, PAGETABLE_FILEMAPPED) &&
				pageDir_isPageFlagSet(*pageEntry, PAGETABLE_KERNEL))
			{
				*pageEntry = 0;
			}
		}
		current += 0x1000;
	}

	return 0;
}

int proc_removeProc(
	proc_meta_t *meta,
	nvmos_dl_allocator_t *alloc)
{
	proc_freeBlks(meta, 0, 0xFFFFFFFF, alloc);
	pageDir_free((pageDir_t *)(meta->pageDir), alloc);

	return 0;
}