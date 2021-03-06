#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/kdef.h>

int nvmos_dl_alloc_createAllocator(
    nvmos_dl_allocator_t *allocator,
    nvmos_ptr_t startAddress,
    size_t length,
    size_t allocationBlockSize)
{
    nvmos_ptr_t realStartAddr =
        startAddress % allocationBlockSize;
    if (realStartAddr != 0)
    {
        length -= realStartAddr;
        realStartAddr = allocationBlockSize - realStartAddr;
    }
    realStartAddr += startAddress;

    length -= length % allocationBlockSize;

    uint32_t blockCount = length / allocationBlockSize;
    if (blockCount < 2)
    {
        return -1;
    }
    allocator->allocationBlockSize = allocationBlockSize;

    nvmos_dl_freeBlockNode_t *headNode =
        (nvmos_dl_freeBlockNode_t *)realStartAddr;
    nvmos_dl_freeBlockNode_t *tailNode =
        (nvmos_dl_freeBlockNode_t *)(realStartAddr +
                                     (blockCount - 1) *
                                         allocationBlockSize);

    headNode->sameValueNext = NULL;
    headNode->redBlackTreeNode.parent = NULL;
    headNode->redBlackTreeNode.leftChild = NULL;
    headNode->redBlackTreeNode.rightChild = NULL;
    headNode->redBlackTreeNode.value = blockCount;
    headNode->redBlackTreeNode.content =
        (nvmos_ptr_t)headNode;
    headNode->segmentHead = (nvmos_ptr_t)headNode;
    headNode->segmentTail = (nvmos_ptr_t)tailNode;

    tailNode->sameValueNext = NULL;
    tailNode->redBlackTreeNode.parent = NULL;
    tailNode->redBlackTreeNode.leftChild = NULL;
    tailNode->redBlackTreeNode.rightChild = NULL;
    tailNode->redBlackTreeNode.value = blockCount;
    tailNode->redBlackTreeNode.content =
        (nvmos_ptr_t)headNode;
    headNode->segmentHead = (nvmos_ptr_t)headNode;
    headNode->segmentTail = (nvmos_ptr_t)tailNode;

    allocator->head = NULL;
    rbt_insertNode(
        &(allocator->head),
        (&headNode->redBlackTreeNode));

    return 0;
}

int nvmos_dl_alloc_retrieveAllocator(
    nvmos_dl_allocator_t *allocator,
    nvmos_ptr_t head,
    size_t allocationBlockSize)
{
    allocator->allocationBlockSize = allocationBlockSize;
    allocator->head =
        (rbt_node_t *)head;
    return 0;
}

nvmos_ptr_t nvmos_dl_alloc_allocateBlocks(
    nvmos_dl_allocator_t *allocator,
    size_t blockCount)
{
    rbt_node_t *target =
        rbt_findNode(allocator->head, blockCount, false, true);
    if (target == NULL)
    {
        return NULL;
    }
    size_t gotBlockCount = target->value;
    nvmos_dl_freeBlockNode_t *targetNode =
        (nvmos_dl_freeBlockNode_t *)target->content;
    if (targetNode == NULL)
    {
        return NULL;
    }

    if (targetNode->sameValueNext == NULL)
    {
        rbt_removeNode(&(allocator->head), target);
    }
    else
    {
        nvmos_dl_freeBlockNode_t *tmp = NULL;
        while (((nvmos_dl_freeBlockNode_t *)(targetNode->sameValueNext))->sameValueNext != NULL)
        {
            tmp = targetNode;
            targetNode =
                (nvmos_dl_freeBlockNode_t *)(targetNode->sameValueNext);
        }
        if (tmp != NULL)
            tmp->sameValueNext = NULL;
    }

    if (gotBlockCount > blockCount)
    {
        size_t blocksLeft = gotBlockCount - blockCount;
        nvmos_ptr_t newStartBlock =
            (nvmos_ptr_t)targetNode +
            blockCount * allocator->allocationBlockSize;
        nvmos_dl_alloc_deallocateBlocks(
            allocator,
            newStartBlock,
            blocksLeft);
    }

    return (nvmos_ptr_t)targetNode;
}

int nvmos_dl_alloc_deallocateBlocks(
    nvmos_dl_allocator_t *allocator,
    nvmos_ptr_t startBlock,
    size_t length)
{
    nvmos_dl_freeBlockNode_t *newNode =
        (nvmos_dl_freeBlockNode_t *)startBlock;
    nvmos_dl_freeBlockNode_t *endNode =
        (nvmos_dl_freeBlockNode_t *)(startBlock +
                                     (length - 1) *
                                         allocator->allocationBlockSize);

    nvmos_dl_freeBlockNode_t *oneBlockBefore =
        (nvmos_dl_freeBlockNode_t *)(startBlock -
                                     allocator->allocationBlockSize);
    nvmos_dl_freeBlockNode_t *oneBlockAfter =
        (nvmos_dl_freeBlockNode_t *)(startBlock +
                                     length *
                                         allocator->allocationBlockSize);

    // TODO Depends on destroying tail block on allocation
    if (oneBlockBefore->segmentTail ==
            (nvmos_ptr_t)oneBlockBefore &&
        oneBlockBefore->segmentHead ==
            oneBlockBefore->segmentTail -
                (oneBlockBefore->redBlackTreeNode.value - 1) *
                    allocator->allocationBlockSize)
    {
        size_t previousSegmentLength =
            oneBlockBefore->redBlackTreeNode.value;
        rbt_node_t *segmentBefore =
            rbt_findNode(
                allocator->head,
                previousSegmentLength,
                true,
                true);
        if (segmentBefore != NULL)
        {
            nvmos_ptr_t sameValuePrevious = NULL;
            nvmos_ptr_t targetNode =
                segmentBefore->content;
            bool found = false;

            while (targetNode != NULL &&
                   targetNode != oneBlockBefore->segmentHead)
            {
                sameValuePrevious = targetNode;
                targetNode =
                    ((nvmos_dl_freeBlockNode_t *)targetNode)->sameValueNext;
            }
            if (targetNode != NULL &&
                sameValuePrevious == NULL)
            {
                rbt_removeNode(
                    &(allocator->head),
                    &(((nvmos_dl_freeBlockNode_t *)targetNode)->redBlackTreeNode));
                found = true;
            }
            else if (targetNode != NULL &&
                     sameValuePrevious != NULL)
            {
                ((nvmos_dl_freeBlockNode_t *)sameValuePrevious)->sameValueNext =
                    ((nvmos_dl_freeBlockNode_t *)targetNode)->sameValueNext;
                found = true;
            }
            if (found)
            {
                length += previousSegmentLength;
                newNode = (nvmos_dl_freeBlockNode_t *)targetNode;
            }
        }
    }

    if (oneBlockAfter->segmentHead ==
            (nvmos_ptr_t)oneBlockAfter &&
        oneBlockAfter->segmentTail ==
            (nvmos_ptr_t)oneBlockAfter +
                (oneBlockAfter->redBlackTreeNode.value - 1) *
                    allocator->allocationBlockSize)
    {
        size_t nextSegmentLength =
            oneBlockAfter->redBlackTreeNode.value;
        rbt_node_t *segmentAfter =
            rbt_findNode(
                allocator->head,
                nextSegmentLength,
                true,
                true);
        if (segmentAfter != NULL)
        {
            nvmos_ptr_t sameValuePrevious = NULL;
            nvmos_ptr_t targetNode =
                segmentAfter->content;
            bool found = false;

            while (targetNode != NULL &&
                   targetNode != oneBlockAfter->segmentHead)
            {
                sameValuePrevious = targetNode;
                targetNode =
                    ((nvmos_dl_freeBlockNode_t *)targetNode)->sameValueNext;
            }
            if (targetNode != NULL &&
                sameValuePrevious == NULL)
            {
                rbt_removeNode(
                    &(allocator->head),
                    &(((nvmos_dl_freeBlockNode_t *)targetNode)->redBlackTreeNode));
                found = true;
            }
            else if (targetNode != NULL &&
                     sameValuePrevious != NULL)
            {
                ((nvmos_dl_freeBlockNode_t *)sameValuePrevious)->sameValueNext =
                    ((nvmos_dl_freeBlockNode_t *)targetNode)->sameValueNext;
                found = true;
            }
            if (found)
            {
                length += nextSegmentLength;
                endNode =
                    (nvmos_dl_freeBlockNode_t *)((nvmos_dl_freeBlockNode_t *)targetNode)->segmentTail;
            }
        }
    }

    newNode->sameValueNext = NULL;
    newNode->segmentHead = (nvmos_ptr_t)newNode;
    newNode->segmentTail = (nvmos_ptr_t)endNode;
    newNode->redBlackTreeNode.parent = NULL;
    newNode->redBlackTreeNode.leftChild = NULL;
    newNode->redBlackTreeNode.rightChild = NULL;
    newNode->redBlackTreeNode.redBlackFlag = rbt_RED;
    newNode->redBlackTreeNode.value = length;
    newNode->redBlackTreeNode.content = (nvmos_ptr_t)newNode;

    endNode->sameValueNext = NULL;
    endNode->segmentHead = (nvmos_ptr_t)newNode;
    endNode->segmentTail = (nvmos_ptr_t)endNode;
    endNode->redBlackTreeNode.parent = NULL;
    endNode->redBlackTreeNode.leftChild = NULL;
    endNode->redBlackTreeNode.rightChild = NULL;
    endNode->redBlackTreeNode.redBlackFlag = rbt_RED;
    endNode->redBlackTreeNode.value = length;
    endNode->redBlackTreeNode.content = (nvmos_ptr_t)newNode;

    rbt_node_t *sameLengthNode =
        rbt_findNode(
            allocator->head,
            length,
            true,
            true);
    if (sameLengthNode != NULL)
    {
        nvmos_dl_freeBlockNode_t *node =
            (nvmos_dl_freeBlockNode_t *)sameLengthNode->content;
        while (node->sameValueNext != NULL)
        {
            node = (nvmos_dl_freeBlockNode_t *)node->sameValueNext;
        }
        node->sameValueNext = (nvmos_ptr_t)newNode;
    }
    else
    {
        rbt_insertNode(
            &(allocator->head),
            &(newNode->redBlackTreeNode));
    }

    return 0;
}