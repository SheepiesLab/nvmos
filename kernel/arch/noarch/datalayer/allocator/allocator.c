#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/kdef.h>

int nvmos_dl_alloc_createAllocator(
    nvmos_dl_allocator_t *allocator,
    nvmos_pointer_t startAddress,
    size_t length,
    size_t allocationBlockSize)
{
    nvmos_pointer_t realStartAddr =
        startAddress % allocationBlockSize;
    if (realStartAddr != 0)
    {
        length -= realStartAddr;
        realStartAddr = allocationBlockSize - realStartAddr;
    }
    realStartAddr += startAddress;

    length = length % allocationBlockSize;

    uint32_t blockCount = length / allocationBlockSize;
    if (blockCount < 2)
    {
        return -1;
    }
    allocator->allocationBlockSize = allocationBlockSize;

    nvmos_dl_freeBlockListNode_t *headNode =
        (nvmos_dl_freeBlockListNode_t *)realStartAddr;
    nvmos_dl_freeBlockListNode_t *tailNode =
        (nvmos_dl_freeBlockListNode_t *)(realStartAddr +
                                         (blockCount - 1) *
                                             allocationBlockSize);

    headNode->sameValueNext = NULL;
    headNode->redBlackTreeNode.parent = NULL;
    headNode->redBlackTreeNode.leftChild = NULL;
    headNode->redBlackTreeNode.rightChild = NULL;
    headNode->redBlackTreeNode.value = blockCount;
    headNode->redBlackTreeNode.content =
        (nvmos_pointer_t)headNode;
    headNode->segmentHead = (nvmos_pointer_t)headNode;
    headNode->segmentTail = (nvmos_pointer_t)tailNode;

    tailNode->sameValueNext = NULL;
    tailNode->redBlackTreeNode.parent = NULL;
    tailNode->redBlackTreeNode.leftChild = NULL;
    tailNode->redBlackTreeNode.rightChild = NULL;
    tailNode->redBlackTreeNode.value = blockCount;
    tailNode->redBlackTreeNode.content =
        (nvmos_pointer_t)headNode;
    headNode->segmentHead = (nvmos_pointer_t)headNode;
    headNode->segmentTail = (nvmos_pointer_t)tailNode;

    rbt_insertNode(
        &(allocator->head),
        (&headNode->redBlackTreeNode));

    return 0;
}

int nvmos_dl_alloc_retrieveAllocator(
    nvmos_dl_allocator_t *allocator,
    nvmos_pointer_t head,
    size_t allocationBlockSize)
{
    allocator->allocationBlockSize = allocationBlockSize;
    allocator->head =
        (nvmos_dl_freeBlockListNode_t *)head->redBlackTreeNode;
    return 0;
}

nvmos_pointer_t nvmos_dl_alloc_allocateBlocks(
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
    nvmos_dl_freeBlockListNode_t *targetNode =
        (nvmos_dl_freeBlockListNode_t *)target->content;
    if (targetNode == NULL)
    {
        return NULL;
    }
    if (targetNode->sameValueNext == NULL)
    {
        rbt_removeNode(target);
    }
    else
    {
        nvmos_dl_freeBlockListNode_t *tmp;
        while (targetNode->sameValueNext->sameValueNext != NULL)
        {
            tmp = targetNode;
            targetNode = targetNode->sameValueNext;
        }
        tmp->sameValueNext = NULL;
    }
    if (gotBlockCount > blockCount)
    {
        size_t blocksLeft = gotBlockCount - blockCount;
        
    }
}

int nvmos_dl_alloc_deallocateBlocks(
    nvmos_dl_allocator_t *allocator,
    nvmos_pointer_t startBlock,
    size_t length)
{
}