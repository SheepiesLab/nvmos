#include <kernel/datalayer/allocator.h>

int nvmos_dl_alloc_createAllocator(
	nvmos_dl_allocator_t *	allocator,
	nvmos_pointer			startAddress,
	nvmos_size			length,
	nvmos_size			allocationBlockSize
	)
{
	{
		nvmos_size startOffset = startAddress % allocationBlockSize;
		if (startOffset != 0) {
			startOffset = allocationBlockSize - startOffset;
			startAddress += startOffset;
			if (length <= startOffset)
				return -1;
			length -= startOffset;
		}
		length -= length % allocationBlockSize;
	}
	nvmos_size blockLength = length / allocationBlockSize;

	if (blockLength < 2)
		return -1;

	nvmos_dl_freeBlockListNode_t *startBlock =
		(nvmos_dl_freeBlockListNode_t *)startAddress;
	nvmos_dl_freeBlockListNode_t *endBlock =
		(nvmos_dl_freeBlockListNode_t *)
		(startAddress + length - allocationBlockSize);

	startBlock->length = blockLength;
	startBlock->flags =
		(
			NVMOS_DL_ALLOC_INTREE_NODE |
			NVMOS_DL_ALLOC_BLACK_NODE |
			NVMOS_DL_ALLOC_SEGMENT_HEAD_NODE
		);
	startBlock->sameValueNext = NULL;
	startBlock->parent = NULL;
	startBlock->leftChild = NULL;
	startBlock->rightChild = NULL;

	endBlock->length = blockLength;
	endBlock->flags =
		(
			NVMOS_DL_ALLOC_INTREE_NODE |
			NVMOS_DL_ALLOC_BLACK_NODE |
			NVMOS_DL_ALLOC_SEGMENT_TAIL_NODE
		);
	endBlock->sameValueNext = NULL;
	endBlock->parent = (nvmos_pointer)startBlock;
	endBlock->leftChild = NULL;
	endBlock->rightChild = NULL;

	allocator->allocationBlockSize = allocationBlockSize;
	allocator->head = startBlock;

	return 0;
}



int nvmos_dl_alloc_retrieveAllocator(
	nvmos_dl_allocator_t *	allocator,
	nvmos_pointer			head,
	nvmos_size			allocationBlockSize
	)
{
	freeBlockList->allocationBlockSize = allocationBlockSize;
	freeBlockList->head = (nvmos_dl_freeBlockListNode_t *)head;

	return 0;
}



nvmos_pointer nvmos_dl_alloc_allocateBlocks(
	nvmos_dl_allocator_t *	allocator,
	nvmos_size			blockCount
	)
{
}



int nvmos_dl_alloc_deallocateBlocks(
	nvmos_dl_allocator_t *	allocator,
	nvmos_pointer			startBlock,
	nvmos_size			length
	)
{
}