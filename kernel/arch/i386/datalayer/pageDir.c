#include <kernel/datalayer/pageDir.h>

bool pageDir_isSegmentUnmapped(
	pageDir_t *pageDir,
	nvmos_ptr_t start,
	size_t blockLength)
{
	size_t i;
	start = start & 0xFFFFF000;

	for (; addr < addr + blockLength * 0x1000;)
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
	nvmos_ptr_t blockLength,
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
	nvmos_ptr_t end = start + blockLength * 0x1000;
	nvmos_ptr_t current = start;

	// Pre-allocate page tables
	for (; current < end; current += 0x400000)
	{
		size_t pageTableIdx = current >> 22 % 0x400;
		if (pageDir->page_tbs[pageTableIdx] & PAGEDIR_PRESENT)
		{
			// Do nothing
		}
		else
		{
			pageDir->page_tbs[pageTableIdx] =
				nvmos_dl_alloc_allocateBlocks(allocator, 1);
			pageDir->page_tbs[pageTableIdx] |= PAGEDIR_PRESENT;
		}
	}

	return 0;
}