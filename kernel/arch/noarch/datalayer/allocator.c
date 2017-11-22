
#include <kernel/datalayer/allocator.h>

int DL_allocator_createAllocator(
  datalayer_allocator_t *allocator,
  kptr_t startAddress,
  size_t length,
  size_t allocationBlockSize
){
  {
    size_t startOffset = startAddress % allocationBlockSize;
    if (startOffset != 0){
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

  datalayer_freeBlockListNode_t *startBlock =
    (datalayer_freeBlockListNode_t *)startAddress;
  datalayer_freeBlockListNode_t *endBlock =
    (datalayer_freeBlockListNode_t *)
      (startAddress + length - allocationBlockSize);

  startBlock->length = blockLength;
  startBlock->flags =
    (DL_ALLOC_INTREE_NODE | DL_ALLOC_BLACK_NODE | DL_ALLOC_SEGMENT_HEAD_NODE);
  startBlock->sameValueNext = NULL;
  startBlock->parent = NULL;
  startBlock->leftChild = NULL;
  startBlock->rightChild = NULL;

  endBlock->length = blockLength;
  endBlock->flags =
    (DL_ALLOC_INTREE_NODE | DL_ALLOC_BLACK_NODE | DL_ALLOC_SEGMENT_TAIL_NODE);
  endBlock->sameValueNext = NULL;
  endBlock->parent = (kptr_t)startBlock;
  endBlock->leftChild = NULL;
  endBlock->rightChild = NULL;

  freeBlockList->allocationBlockSize = allocationBlockSize;
  freeBlockList->head = startBlock;

  return 0;
}



int DL_allocator_retrieveAllocator(
  datalayer_allocator_t *allocator,
  kptr_t head,
  size_t allocationBlockSize
){
  freeBlockList->allocationBlockSize = allocationBlockSize;
  freeBlockList->head = (datalayer_freeBlockListNode_t *)head;

  return 0;
}



kptr_t DL_allocator_allocateBlocks(
  datalayer_allocator_t *allocator,
  size_t blockCount
){
  
}
