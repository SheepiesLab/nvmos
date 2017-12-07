#include <kernel/datalayer/allocator/redBlackTree/rbtHelper.h>

int leftRotateNode(rbt_node_t **head, rbt_node_t *target)
{
    rbt_nodeProperty_t targetProperty =
        rbt_getNodeProperty(*head, target);

    if (targetProperty == rbt_CORRUPTED ||
        targetProperty == rbt_ERROR)
    {
        return -1;
    }
    if (target->rightChild == NULL)
    {
        return -2;
    }

    // Change parent to point to self's right child
    if (targetProperty == rbt_ROOT)
    {
        *head = target->rightChild;
        target->rightChild->parent = NULL;
    }
    else if (targetProperty == rbt_LEFTCHILD)
    {
        target->parent->leftChild = target->rightChild;
        target->rightChild->parent = target->parent;
    }
    else if (targetProperty == rbt_RIGHTCHILD)
    {
        target->parent->rightChild = target->rightChild;
        target->rightChild->parent = target->parent;
    }

    // Target's parent is original right child
    target->parent = target->rightChild;

    // Target's right child is original right child's left child
    target->rightChild = target->rightChild->leftChild;
    if (target->rightChild != NULL)
    {
        target->rightChild->parent = target;
    }

    // Left child of original right child is target
    target->parent->leftChild = target;

    return 0;
}

int rightRotateNode(rbt_node_t **head, rbt_node_t *target)
{
    rbt_nodeProperty_t targetProperty =
        rbt_getNodeProperty(*head, target);

    if (targetProperty == rbt_CORRUPTED ||
        targetProperty == rbt_ERROR)
    {
        return -1;
    }
    if (target->leftChild == NULL)
    {
        return -2;
    }

    // Change parent to point to self's left child
    if (targetProperty == rbt_ROOT)
    {
        *head = target->leftChild;
        target->leftChild->parent = NULL;
    }
    else if (targetProperty == rbt_LEFTCHILD)
    {
        target->parent->leftChild = target->leftChild;
        target->leftChild->parent = target->parent;
    }
    else if (targetProperty == rbt_RIGHTCHILD)
    {
        target->parent->rightChild = target->leftChild;
        target->leftChild->parent = target->parent;
    }

    // Target's parent is original left child
    target->parent = target->leftChild;

    // Target's left child is original left child's right child
    target->leftChild = target->leftChild->rightChild;
    if (target->leftChild != NULL)
    {
        target->leftChild->parent = target;
    }

    // Right child of original left child is target
    target->parent->rightChild = target;

    return 0;
}

int swapNode(rbt_node_t **head, rbt_node_t *a, rbt_node_t *b)
{
    // Red-black Tree Corrupted
    rbt_nodeProperty_t aProperty = rbt_getNodeProperty(*head, a);
    rbt_nodeProperty_t bProperty = rbt_getNodeProperty(*head, b);
    if (aProperty == rbt_ERROR ||
        aProperty == rbt_CORRUPTED ||
        bProperty == rbt_ERROR ||
        bProperty == rbt_CORRUPTED ||
        (aProperty == rbt_ROOT && bProperty == rbt_ROOT))
    {
        return -1;
    }

    rbt_node_t *tmp;

    // Swap Parent
    if (aProperty == rbt_ROOT)
    {
        *head = b;
        a->parent = b->parent;
        b->parent = NULL;
        if (bProperty == rbt_LEFTCHILD)
        {
            a->parent->leftChild = a;
        }
        else if (bProperty == rbt_RIGHTCHILD)
        {
            a->parent->rightChild = a;
        }
        else
            return -1;
    }
    else if (bProperty == rbt_ROOT)
    {
        *head = a;
        b->parent = a->parent;
        a->parent = NULL;
        if (aProperty == rbt_LEFTCHILD)
        {
            b->parent->leftChild = b;
        }
        else if (aProperty == rbt_RIGHTCHILD)
        {
            b->parent->rightChild = b;
        }
        else
            return -1;
    }
    else
    {
        tmp = a->parent;
        a->parent = b->parent;
        b->parent = tmp;

        if (aProperty == rbt_LEFTCHILD)
        {
            b->parent->leftChild = b;
        }
        else if (aProperty == rbt_RIGHTCHILD)
        {
            b->parent->rightChild = b;
        }
        else
            return -1;

        if (bProperty == rbt_LEFTCHILD)
        {
            a->parent->leftChild = a;
        }
        else if (bProperty == rbt_RIGHTCHILD)
        {
            a->parent->rightChild = a;
        }
        else
            return -1;
    }

    // Swap Left Child
    tmp = a->leftChild;
    a->leftChild = b->leftChild;
    b->leftChild = tmp;

    if (a->leftChild != NULL)
        a->leftChild->parent = a;
    if (b->leftChild != NULL)
        b->leftChild->parent = b;

    // Swap Right Child
    tmp = a->rightChild;
    a->rightChild = b->rightChild;
    b->rightChild = tmp;

    if (a->rightChild != NULL)
        a->rightChild->parent = a;
    if (b->rightChild != NULL)
        b->rightChild->parent = b;

    return 0;
}

int swapNodeColor(rbt_node_t *a, rbt_node_t *b)
{
    rbt_redBlackFlag_t tmp;
    tmp = a->redBlackFlag;
    a->redBlackFlag = b->redBlackFlag;
    b->redBlackFlag = tmp;
}

rbt_node_t *getUncle(rbt_node_t *head, rbt_node_t *target)
{
    rbt_nodeProperty_t targetProperty =
        rbt_getNodeProperty(head, target);
    if (targetProperty == rbt_ERROR ||
        targetProperty == rbt_CORRUPTED)
    {
        return NULL;
    }
    if (targetProperty == rbt_ROOT)
    {
        return NULL;
    }
    rbt_nodeProperty_t parentProperty =
        rbt_getNodeProperty(head, target->parent);
    if (parentProperty == rbt_ERROR ||
        parentProperty == rbt_CORRUPTED)
    {
        return NULL;
    }
    if (parentProperty == rbt_ROOT)
    {
        return NULL;
    }
    if (parentProperty == rbt_LEFTCHILD)
    {
        return target->parent->parent->rightChild;
    }
    else if (parentProperty == rbt_RIGHTCHILD)
    {
        return target->parent->parent->leftChild;
    }
    return NULL;
}

rbt_node_t *getSibling(rbt_node_t *head, rbt_node_t *target)
{
    rbt_nodeProperty_t targetProperty =
        rbt_getNodeProperty(head, target);
    if (targetProperty == rbt_ERROR ||
        targetProperty == rbt_CORRUPTED ||
        targetProperty == rbt_ROOT)
    {
        return NULL;
    }

    else if (targetProperty == rbt_LEFTCHILD)
    {
        return target->parent->rightChild;
    }
    else if (targetProperty == rbt_RIGHTCHILD)
    {
        return target->parent->leftChild;
    }

    return NULL;
}

rbt_redBlackFlag_t getNodeColor(rbt_node_t *target)
{
    if (target == NULL || target->redBlackFlag == rbt_BLACK)
    {
        return rbt_BLACK;
    }
    return rbt_RED;
}

#ifndef NVMOS
#define OS_DEPENDENT
#include <stdio.h>
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
void printTree(rbt_node_t *head, unsigned int level)
{
    if (head->redBlackFlag == rbt_BLACK)
    {
        printf("%s%3u%s | ", KBLU, head->value, KNRM);
    }
    else if (head->redBlackFlag == rbt_RED)
    {
        printf("%s%3u%s | ", KRED, head->value, KNRM);
    }

    if (head->rightChild != NULL)
    {
        printTree(head->rightChild, level + 1);
    }
    else
    {
        printf("--- | ");
    }
    printf("\n");
    for (int i = 0; i < level + 1; ++i)
    {
        printf("    | ");
    }
    if (head->leftChild != NULL)
    {

        printTree(head->leftChild, level + 1);
    }
    else
    {
        printf("--- | ");
    }
}
#endif