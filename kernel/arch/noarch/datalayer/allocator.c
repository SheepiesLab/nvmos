#include <kernel/datalayer/allocator.h>
#include <kernel/kdef.h>

int nvmos_dl_alloc_createAllocator(
	nvmos_dl_allocator_t *allocator,
	nvmos_pointer_t startAddress,
	size_t length,
	size_t allocationBlockSize)
{
	{
		nvmos_size startOffset =
			startAddress % allocationBlockSize;
		if (startOffset != 0)
		{
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
		(nvmos_dl_freeBlockListNode_t *)(startAddress +
										 length -
										 allocationBlockSize);

	startBlock->length = blockLength;
	startBlock->flags =
		(NVMOS_DL_ALLOC_INTREE_NODE |
		 NVMOS_DL_ALLOC_BLACK_NODE |
		 NVMOS_DL_ALLOC_SEGMENT_HEAD_NODE);
	startBlock->sameValueNext = NULL;
	startBlock->parent = NULL;
	startBlock->leftChild = NULL;
	startBlock->rightChild = NULL;

	endBlock->length = blockLength;
	endBlock->flags =
		(NVMOS_DL_ALLOC_INTREE_NODE |
		 NVMOS_DL_ALLOC_BLACK_NODE |
		 NVMOS_DL_ALLOC_SEGMENT_TAIL_NODE);
	endBlock->sameValueNext = NULL;
	endBlock->parent = (nvmos_pointer)startBlock;
	endBlock->leftChild = NULL;
	endBlock->rightChild = NULL;

	allocator->allocationBlockSize = allocationBlockSize;
	allocator->head = startBlock;

	return 0;
}

int nvmos_dl_alloc_retrieveAllocator(
	nvmos_dl_allocator_t *allocator,
	nvmos_pointer_t head,
	size_t allocationBlockSize)
{
	freeBlockList->allocationBlockSize = allocationBlockSize;
	freeBlockList->head = (nvmos_dl_freeBlockListNode_t *)head;

	return 0;
}

nvmos_dl_freeBlockListNode_t *_findFittestNode(
	nvmos_dl_freeBlockListNode_t *head,
	size_t blockCount)
{
	if (head == NULL)
		return NULL;

	if (head->length == blockCount)
		return head;

	if (head->length < blockCount)
	{
		if (head->rightChild == NULL)
			return NULL;
		return _findFittestNode(head->rightChild, blockCount);
	}

	if (head->length > blockCount)
	{
		if (head->leftChild == NULL)
			return head;
		nvmos_dl_freeBlockListNode_t *candidate =
			_findFittestNode(head->leftChild, blockCount);
		if (candidate == NULL)
			return head;
		if (candidate->length > head->length)
			return head;
		else
			return candidate;
	}

	return NULL;
}

#define SWAPNODE_A_NOPARENT 0x1
#define SWAPNODE_B_NOPARENT 0x2
#define SWAPNODE_A_PARENTLINKCORRUPTED 0x4
#define SWAPNODE_B_PARENTLINKCORRUPTED 0x8
uint32_t _swapNode(nvmos_dl_freeBlockListNode_t *a,
				   nvmos_dl_freeBlockListNode_t *b)
{
	uint32_t returnFlags = 0;

	nvmos_dl_freeBlockListNode_t *tmp;

	// Swap parent linkage
	if (a->parent->leftChild != a &&
		a->parent->rightChild != a)
	{
		return SWAPNODE_A_PARENTLINKCORRUPTED;
	}
	else if (a->parent->leftChild == a &&
			 a->parent->rightChild == a)
	{
		return SWAPNODE_A_PARENTLINKCORRUPTED;
	}

	if (b->parent->leftChild != b &&
		b->parent->rightChild != b)
	{
		return SWAPNODE_B_PARENTLINKCORRUPTED;
	}
	else if (b->parent->leftChild == b &&
			 b->parent->rightChild == b)
	{
		return SWAPNODE_B_PARENTLINKCORRUPTED;
	}

	if (a->parent->leftChild == a)
	{
		a->parent->leftChild = b;
	}
	else if (a->parent->rightChild == a)
	{
		a->parent->rightChild = b
	}

	if (b->parent->leftChild == b)
	{
		b->parent->leftChild = a;
	}
	else if (b->parent->rightChild == b)
	{
		b->parent->rightChild = a;
	}

	tmp = a->parent;
	a->parent = b->parent;
	b->parent = tmp;

	// Swap left child linkage
	
}

int _popNode(nvmos_dl_freeBlockListNode_t **head,
			 nvmos_dl_freeBlockListNode_t *target)
{
	nvmos_dl_freeBlockListNode_t *leftChild, rightChild, parent;
	leftChild = target->leftChild;
	rightChild = target->rightChild;
	parent = target->parent;

	if (leftChild == NULL && rightChild == NULL)
	{
	}

	if (leftChild == NULL)
	{
	}

	if (rightChild == NULL)
	{
	}

	if (leftChild != NULL && rightChild != NULL)
	{
		nvmos_dl_freeBlockListNode_t *inOrderSuccessor;
		inOrderSuccessor = rightChild;
		while (inOrderSuccessor->leftChild != NULL)
		{
			inOrderSuccessor = inOrderSuccessor->leftChild;
		}
	}

	return -1;
}

nvmos_pointer nvmos_dl_alloc_allocateBlocks(
	nvmos_dl_allocator_t *allocator,
	size_t blockCount)
{
	nvmos_dl_freeBlockListNode_t **head = &(allocator->head);
	nvmos_dl_freeBlockListNode_t *selectedNode =
		_findFittestNode(*head, blockCount);
	if (selectedNode == NULL)
		return NULL;

	if (selectedNode->sameValueNext == NULL)
	{
		_popNode(head, selectedNode);
	}

	while (selectedNode->sameValueNext != NULL)
		selectedNode = selectedNode->sameValueNext;
}

int nvmos_dl_alloc_deallocateBlocks(
	nvmos_dl_allocator_t *allocator,
	nvmos_pointer startBlock,
	nvmos_size length)
{
}
