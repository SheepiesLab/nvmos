//
// Created by benng on 7/18/17.
//

#include <kernel/mman/HeapManager.h>
#include <string.h>

/**
 * Heap Management
 *
 * Heap is managed by a linked list.
 * Linked list node is located in the heap, right after the allocated memory.
 * Allocation list nodes' address includes the node space itself.
 *
 * endAddr - startAddr = sizeOfContent + sizeOfNode
 *
 * Memory is allocated with first-fit policy.
 * Heap cannot overflow as it would not allocate when there is no space.
 */

struct heapman {
    HeapAllocationListNode *head;
    kptr_t heapStart;
    kptr_t heapEnd;
};

struct heapAllocListNode {
    kptr_t startAddr;
    kptr_t endAddr;
    HeapAllocationListNode *next;
};

int heapman_construct(HeapManager *heapman, kptr_t heapStart, kptr_t heapEnd) {
    heapman->heapStart = heapStart;
    heapman->heapEnd = heapEnd;
    head = NULL;
    return 0;
}

kptr_t heapman_malloc(HeapManager *heapman, size_t size) {
    if (heapman == NULL) return NULL;

    size_t neededSpace = size + sizeof(struct heapAllocListNode);

    struct heapAllocListNode *newNode;

    if (heapman->head == NULL) {
        if (neededSpace < (heapman->heapEnd - heapman->heapStart)) {
            newNode = heapman->heapStart + size;
            heapman->head = newNode;
            newNode->startAddr = heapman->heapStart;
            newNode->endAddr = heapman->heapStart + neededSpace;
            return newNode->startAddr;
        } else {
            // TODO: not enough heap return NULL
            return NULL;
        }
    } else if (heapman->head->startAddr - heapman->heapStart > neededSpace) {
        newNode = heapman->heapStart + size;
        newNode->next = heapman->head;
        heapman->head = newNode;
        newNode->startAddr = heapman->heapStart;
        newNode->endAddr = heapman->heapStart + neededSpace;
        return newNode->startAddr;
    } else {
        HeapAllocationListNode *currentNode = heapman->head;
        while (currentNode->next != null) {
            if (
                    currentNode->next->startAddr - currentNode->endAddr >
                    neededSpace) {
                newNode = currentNode->endAddr + size;
                newNode->startAddr = currentNode->endAddr;
                newNode->endAddr = currentNode->startAddr + neededSpace;
                newNode->next = currentNode->next;
                currentNode->next = newNode;
                return newNode->startAddr;
            }
            currentNode = currentNode->next;
        }
        if (neededSpace < (heapman->heapEnd - currentNode->endAddr)) {
            newNode = currentNode->endAddr + size;
            currentNode->next = newNode;
            newNode->next = NULL;
            newNode->startAddr = currentNode->endAddr;
            newNode->endAddr = currentNode->endAddr + neededSpace;
            return newNode->startAddr;
        } else {
            // TODO: not enough heap return NULL
            return NULL;
        }
    }
}

kptr_t heapman_calloc(HeapManager *heapman, size_t len, size_t size) {
    kptr_t space = heapman_malloc(heapman, len * size);
    if (space != NULL) {
        memset(space, 0, len * size);
        return space;
    }
    return NULL;
}

int heapman_free(HeapManager *heapman, kptr_t loc) {
    if (heapman->head->startAddr == loc){
        heapman->head = heapman->head->next;
        return 0;
    }

    HeapAllocationListNode *currentNode = heapman->head;
    while (currentNode->next != NULL) {
        if (currentNode->next->startAddr > loc){
            return -1;
        }
        if (currentNode->next->startAddr == loc){
            currentNode->next = currentNode->next->next;
            return 0;
        }
        currentNode = currentNode->next;
    };
    return -1;
}