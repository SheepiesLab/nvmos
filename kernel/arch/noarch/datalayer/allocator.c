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

#define SWAPNODE_A_NULL 0x1
#define SWAPNODE_B_NULL 0x2
#define SWAPNODE_A_PARENT_NOTEXIST 0x4
#define SWAPNODE_B_PARENT_NOTEXIST 0x8
#define SWAPNODE_A_PARENT_CORRUPTED 0x10
#define SWAPNODE_B_PARENT_CORRUPTED 0x20
#define SWAPNODE_A_LEFT_CORRUPTED 0x40
#define SWAPNODE_B_LEFT_CORRUPTED 0x80
#define SWAPNODE_A_RIGHT_CORRUPTED 0x100
#define SWAPNODE_B_RIGHT_CORRUPTED 0x200
uint32_t _swapNode(
	nvmos_dl_freeBlockListNode_t **head,
	nvmos_dl_freeBlockListNode_t *a,
	nvmos_dl_freeBlockListNode_t *b)
{
	uint32_t returnFlags = 0;

	nvmos_dl_freeBlockListNode_t *tmp;

	if (a == NULL)
		return SWAPNODE_A_NULL;

	if (b == NULL)
		return SWAPNODE_B_NULL;

	if (a->parent == NULL && a != *head)
		return SWAPNODE_A_PARENT_CORRUPTED;
	else if (a->parent->leftChild != a &&
			 a->parent->rightChild != a)
	{
		return SWAPNODE_A_PARENT_CORRUPTED;
	}
	else if (a->parent->leftChild == a &&
			 a->parent->rightChild == a)
	{
		return SWAPNODE_A_PARENT_CORRUPTED;
	}

	if (b->parent == NULL && b != *head)
		return SWAPNODE_B_PARENT_CORRUPTED;
	else if (b->parent->leftChild != b &&
			 b->parent->rightChild != b)
	{
		return SWAPNODE_B_PARENT_CORRUPTED;
	}
	else if (b->parent->leftChild == b &&
			 b->parent->rightChild == b)
	{
		return SWAPNODE_B_PARENT_CORRUPTED;
	}

	if (a->leftChild != NULL && a->leftChild->parent != a)
		return SWAPNODE_A_LEFT_CORRUPTED;

	if (b->leftChild != NULL && b->leftChild->parent != b)
		return SWAPNODE_B_LEFT_CORRUPTED;

	if (a->rightChild != NULL && a->rightChild->parent != a)
		return SWAPNODE_A_RIGHT_CORRUPTED;

	if (b->rightChild != NULL && b->rightChild->parent != b)
		return SWAPNODE_B_RIGHT_CORRUPTED;

	// Swap parent linkage
	if (a->parent == NULL)
	{
		*head = b;
	}
	else if (a->parent->leftChild == a)
	{
		a->parent->leftChild = b;
	}
	else if (a->parent->rightChild == a)
	{
		a->parent->rightChild = b
	}

	if (b->parent == NULL)
	{
		*head = a;
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
	if (a->leftChild != NULL && b->leftChild != NULL)
	{
		a->leftChild->parent = b;
		b->leftChild->parent = a;

		tmp = a->leftChild;
		a->leftChild = b->leftChild;
		b->leftChild = tmp;
	}
	else if (a->leftChild != NULL && b->leftChild == NULL)
	{
		b->leftChild = a->leftChild;
		b->leftChild->parent = b;
		a->leftChild = NULL;
	}
	else if (a->leftChild == NULL && b->leftChild != NULL)
	{
		a > leftChild = b->leftChild;
		a->leftChild->parent = a;
		b->leftChild = NULL;
	}
	else if (a->leftChild == NULL && b->leftChild == NULL)
	{
	}

	// Swap right child linkage
	if (a->rightChild != NULL && b->rightChild != NULL)
	{
		a->rightChild->parent = b;
		b->rightChild->parent = a;

		tmp = a->rightChild;
		a->rightChild = b->rightChild;
		b->rightChild = tmp;
	}
	else if (a->rightChild != NULL && b->rightChild == NULL)
	{
		b->rightChild = a->rightChild;
		b->rightChild->parent = b;
		a->rightChild = NULL;
	}
	else if (a->rightChild == NULL && b->rightChild != NULL)
	{
		a > rightChild = b->rightChild;
		a->rightChild->parent = a;
		b->rightChild = NULL;
	}
	else if (a->rightChild == NULL && b->rightChild == NULL)
	{
	}

	return 0;
}

int _popNode(nvmos_dl_freeBlockListNode_t **head,
			 nvmos_dl_freeBlockListNode_t *target)
{
	nvmos_dl_freeBlockListNode_t *leftChild, rightChild, parent;
	leftChild = target->leftChild;
	rightChild = target->rightChild;
	parent = target->parent;

	if (leftChild != NULL && rightChild != NULL)
	{
		nvmos_dl_freeBlockListNode_t *inOrderSuccessor;
		inOrderSuccessor = rightChild;
		while (inOrderSuccessor->leftChild != NULL)
		{
			inOrderSuccessor = inOrderSuccessor->leftChild;
		}
		if (_swapNode(head, target, inOrderSuccessor))
			return -1;
		leftChild = target->leftChild;
		rightChild = target->rightChild;
		parent = target->parent;
	}

	if (target->flags & NVMOS_DL_ALLOC_RED_NODE ||
		(target->leftChild != NULL &&
		 target->leftChild->flags & NVMOS_DL_ALLOC_RED_NODE) ||
		(target->rightChild != NULL &&
		 target->rightChild->flags & NVMOS_DL_ALLOC_RED_NODE))
	{
		if (target->parent->leftChild == target)
		{
			target->parent->leftChild = NULL;
		}
		else if (target->parent->rightChild == target)
		{
			target->parent->rightChild = NULL;
		}
		else
			return -1;
	}

	if (leftChild == NULL)
	{
	}

	if (rightChild == NULL)
	{
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
