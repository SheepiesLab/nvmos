#ifndef _PAGEDIR_H_
#define _PAGEDIR_H_

#include <stdint.h>
#include <stdbool.h>

#include <kernel/datalayer/allocator/allocator.h>

typedef struct
{
	uint32_t page_tbs[1024];
} pageDir_t;

typedef struct
{
	uint32_t pages[1024];
} pageTable_t;

// check http://wiki.osdev.org/Paging for page dir

#define PAGEDIR_ADDR_MASK 0xFFFFF000

#define PAGEDIR_PRESENT 0x1
#define PAGEDIR_RW 0x2
#define PAGEDIR_USER 0x4
#define PAGEDIR_WRITETHROUGH 0x8
#define PAGEDIR_NOCACHE 0x10
#define PAGEDIR_ACCESSED 0x20
#define PAGEDIR_BIGPAGE 0x80
#define PAGEDIR_FILEMAPPED 0x200
#define PAGEDIR_KERNEL 0x400

#define PAGETABLE_PRESENT 0x1
#define PAGETABLE_RW 0x2
#define PAGETABLE_USER 0x4
#define PAGETABLE_WRITETHROUGH 0x8
#define PAGETABLE_NOCACHE 0x10
#define PAGETABLE_ACCESSED 0x20
#define PAGETABLE_DIRTY 0x40
#define PAGETABLE_GLOBAL 0x100
#define PAGETABLE_FILEMAPPED 0x200
#define PAGETABLE_KERNEL 0x400

bool isSegmentUnmapped(
	pageDir_t *pageDir,
	nvmos_ptr_t start,
	size_t blockLength);

int pageDir_mapSegment(
	pageDir_t *pageDir,
	nvmos_ptr_t start,
	nvmos_ptr_t blockLength,
	const nvmos_ptr_t *physicalBlocks,
	nvmos_dl_allocator_t *allocator,
	uint32_t pageDirFlags,
	uint32_t pageTableFlags);

#endif