#include <kernel/datalayer/pageDir.h>

bool pageDir_isSegmentUnmapped(
	pageDir_t *pageDir,
	nvmos_ptr_t start,
	size_t blockLength)
{
	start = start & 0xFFFFF000;
	nvmos_ptr_t addr = start;

	for (; addr < start + blockLength * 0x1000;)
	{
		// Disassemble block address into page table / page index
		size_t dirIdx = (addr >> 22) % 0x400;
		size_t tableIdx = (addr >> 12) % 0x400;

		// If this page table is present
		if (pageDir->page_tbs[dirIdx] & PAGEDIR_PRESENT)
		{
			//If this page is present
			pageTable_t *pageTable =
				(pageTable_t *)(pageDir->page_tbs[dirIdx] &
								PAGEDIR_ADDR_MASK);
			if (pageTable->pages[tableIdx] & PAGETABLE_PRESENT)
			{
				return false;
			}

			// Move to next block
			addr += 0x1000;
			continue;
		}
		else
		{
			// Move to next page table
			addr &= 0xFFC00000;
			addr += 0x00400000;
			continue;
		}
	}

	return true;
}

int pageDir_mapSegment(
	pageDir_t *pageDir,
	nvmos_ptr_t start,
	size_t blockLength,
	const nvmos_ptr_t *physicalBlocks,
	nvmos_dl_allocator_t *allocator,
	uint32_t pageDirFlags,
	uint32_t pageTableFlags)
{
	// Check if the paging space is used currently
	// to avoid corrupting old pages
	if (!pageDir_isSegmentUnmapped(pageDir, start, blockLength))
	{
		return -1;
	}

	start &= 0xFFFFF000;
	const nvmos_ptr_t end = start + blockLength * 0x1000;
	nvmos_ptr_t current = start;

	// Pre-allocate page tables
	for (; current < end; current += 0x400000)
	{
		size_t pageTableIdx = current >> 22 % 0x400;
		if (pageDir_isPageTableFlagSet(
				pageDir->page_tbs[pageTableIdx],
				PAGEDIR_PRESENT))
		{
			// Do nothing
		}
		else
		{
			pageDir->page_tbs[pageTableIdx] =
				nvmos_dl_alloc_allocateBlocks(allocator, 1);
			pageDir_setFlag(
				&(pageDir->page_tbs[pageTableIdx]),
				PAGEDIR_PRESENT);
		}
	}

	// Map physical memory to virtual memory
	current = start;
	size_t i = 0;
	for (; current < end; current += 0x1000, ++i)
	{
		size_t pageTableIdx, pageIdx;
		pageDir_entryIdxOf(current, &pageTableIdx, &pageIdx);
		if (pageDir_isPageTableFlagSet(
				pageDir->page_tbs[pageTableIdx],
				PAGEDIR_PRESENT))
		{
			pageDir->page_tbs[pageTableIdx] =
				pageDir_addressOfEntry(pageDir->page_tbs[pageTableIdx]);
			pageDir_setFlag(&(pageDir->page_tbs[pageTableIdx]), pageDirFlags);
			pageDir_setFlag(&(pageDir->page_tbs[pageTableIdx]), PAGEDIR_PRESENT);

			pageTable_t *pageTable =
				(pageTable_t *)(pageDir_addressOfEntry(pageDir->page_tbs[pageTableIdx]));
			if (pageDir_isPageFlagSet(
					pageTable->pages[pageIdx],
					PAGETABLE_PRESENT))
			{
				return -1;
			}
			else
			{
				pageTable->pages[pageIdx] = pageDir_addressOfEntry(physicalBlocks[i]);
				pageDir_setFlag(&(pageTable->pages[pageIdx]), pageTableFlags);
				pageDir_setFlag(&(pageTable->pages[pageIdx]), PAGETABLE_PRESENT);
			}
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

int pageDir_getMap(
	pageDir_t *pageDir,
	nvmos_ptr_t start,
	size_t blockLength,
	nvmos_ptr_t *buffer)
{
	start &= 0xFFFFF000;
	nvmos_ptr_t current = start;
	size_t i = 0;
	for (; i < blockLength; ++i, current += 0x1000)
	{
		size_t pageTableIdx, pageIdx;
		pageDir_entryIdxOf(current, &pageTableIdx, &pageIdx);

		if (pageDir_isPageTableFlagSet(
				pageDir->page_tbs[pageTableIdx],
				PAGEDIR_PRESENT))
		{
			pageTable_t *pageTable =
				pageDir_addressOfEntry(pageDir->page_tbs[pageTableIdx]);
			if (pageDir_isPageFlagSet(pageTable->pages[pageIdx], PAGETABLE_PRESENT))
			{
				buffer[i] = pageDir_addressOfEntry(pageTable->pages[pageIdx]);
			}
			else
			{
				buffer[i] = NULL;
			}
		}
		else
		{
			buffer[i] = NULL;
		}
	}
	return 0;
}

int pageDir_free(
	pageDir_t *pageDir,
	nvmos_dl_allocator_t *allocator)
{
	for (size_t i = 0; i < 0x400; ++i)
	{
		uint32_t *pageTableEntry = &(pageDir->page_tbs[i]);
		if (pageDir_isPageTableFlagSet(*pageTableEntry, PAGEDIR_PRESENT))
		{
			nvmos_ptr_t blockToBeFreed = pageDir_addressOfEntry(*pageTableEntry);
			if (blockToBeFreed != NULL)
			{
				nvmos_dl_alloc_deallocateBlocks(allocator, blockToBeFreed, 1);
				*pageTableEntry = 0;
			}
		}
	}
}