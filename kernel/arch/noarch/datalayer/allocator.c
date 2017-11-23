#include <kernel/datalayer/allocator.h>

int DL_allocator_createAllocator(
	nvmos_dl_allocator_t *	allocator,
	nvmos_ptr_t					startAddress,
	size_t					length,
	size_t					allocationBlockSize
	)
{
	{
		size_t startOffset = startAddress % allocationBlockSize;
		if (startOffset != 0) {
			startOffset = allocationBlockSize - startOffset;
			startAddress += startOffset;
			if (length <= startOffset)
				return -1;
			length -= startOffset;
		}
		length -= length % allocationBlockSize;
	}
	size_t blockLength = length / allocationBlockSize;

	if (blockLength < 2)
		return -1;

	nvmos_dl_freeBlockListNode_t *startBlock =
		(nvmos_dl_freeBlockListNode_t *)startAddress;
	nvmos_dl_freeBlockListNode_t *endBlock =
		(nvmos_dl_freeBlockListNode_t *)
		(startAddress + length - allocationBlockSize);

	startBlock->length = blockLength;
	startBlock->flags =
		(NVMOS_DL_ALLOC_INTREE_NODE | NVMOS_DL_ALLOC_BLACK_NODE | NVMOS_DL_ALLOC_SEGMENT_HEAD_NODE);
	startBlock->sameValueNext = NULL;
	startBlock->parent = NULL;
	startBlock->leftChild = NULL;
	startBlock->rightChild = NULL;

	endBlock->length = blockLength;
	endBlock->flags =
		(NVMOS_DL_ALLOC_INTREE_NODE | NVMOS_DL_ALLOC_BLACK_NODE | NVMOS_DL_ALLOC_SEGMENT_TAIL_NODE);
	endBlock->sameValueNext = NULL;
	endBlock->parent = (nvmos_ptr_t)startBlock;
	endBlock->leftChild = NULL;
	endBlock->rightChild = NULL;

	allocator->allocationBlockSize = allocationBlockSize;
	allocator->head = startBlock;

	return 0;
}



int DL_allocator_retrieveAllocator(
	nvmos_dl_allocator_t *	allocator,
	nvmos_ptr_t					head,
	size_t					allocationBlockSize
	)
{
	freeBlockList->allocationBlockSize = allocationBlockSize;
	freeBlockList->head = (nvmos_dl_freeBlockListNode_t *)head;

	return 0;
}



nvmos_ptr_t DL_allocator_allocateBlocks(
	nvmos_dl_allocator_t *	allocator,
	size_t					blockCount
	)
{
}



int DL_allocator_deallocateBlocks(
	nvmos_dl_allocator_t *	allocator,
	nvmos_ptr_t					startBlock,
	size_t					length
	)
{
}
