#include <kernel/datalayer/allocator/redBlackTree/rbt.h>
#include <kernel/datalayer/allocator/redBlackTree/rbtHelper.h>


rbt_nodeProperty_t rbt_getNodeProperty(
    rbt_node_t *head,
    rbt_node_t *target)
{
    if (head == NULL ||
        target == NULL)
    {
        return rbt_ERROR;
    }

    if (head == target)
    {
        if (target->parent == NULL)
        {
            return rbt_ROOT;
        }
        else
        {
            return rbt_CORRUPTED;
        }
    }
    if (target->parent == NULL)
    {
        return rbt_CORRUPTED;
    }
    if (target->parent->leftChild != NULL)
    {
        if (target->parent->leftChild == target)
        {
            return rbt_LEFTCHILD;
        }
    }
    if (target->parent->rightChild != NULL)
    {
        if (target->parent->rightChild == target)
        {
            return rbt_RIGHTCHILD;
        }
    }
    return rbt_CORRUPTED;
}

int rbt_insertNode(
    rbt_node_t **head,
    rbt_node_t *newNode)
{
    if (newNode->leftChild != NULL ||
        newNode->rightChild != NULL)
    {
        return -1;
    }
    if (*head == NULL)
    {
        *head = newNode;
        newNode->parent = NULL;
        newNode->redBlackFlag = rbt_BLACK;
        return 0;
    }

    rbt_nodeProperty_t headProperty =
        rbt_getNodeProperty(*head, *head);
    if (headProperty != rbt_ROOT)
    {
        return -1;
    }

    rbt_node_t *upperNearest = rbt_findNode(
        *head,
        newNode->value,
        false,
        true);
    if (upperNearest == NULL)
    {
        rbt_node_t *current = *head;
        while (current->rightChild != NULL)
        {
            current = current->rightChild;
        }
        current->rightChild = newNode;
        newNode->parent = current;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }
    else if (upperNearest->value == newNode->value)
    {
        return -2;
    }
    else if (upperNearest->leftChild == NULL)
    {
        upperNearest->leftChild = newNode;
        newNode->parent = upperNearest;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }
    else
    {
        rbt_node_t *current = upperNearest->leftChild;
        while (current->rightChild != NULL)
        {
            current = current->rightChild;
        }
        current->rightChild = newNode;
        newNode->parent = current;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }

    rbt_insertBalance(head, newNode);

    return 0;
}

int rbt_insertBalance(
    rbt_node_t **head,
    rbt_node_t *newNode)
{
    rbt_nodeProperty_t newNodeProperty =
        rbt_getNodeProperty(*head, newNode);
    if (newNodeProperty == rbt_ERROR ||
        newNodeProperty == rbt_CORRUPTED)
    {
        return -1;
    }

    newNode->redBlackFlag = rbt_RED;

    if (newNodeProperty == rbt_ROOT)
    {
        newNode->redBlackFlag = rbt_BLACK;
        return 0;
    }

    rbt_nodeProperty_t parentProperty =
        rbt_getNodeProperty(*head, newNode->parent);
    if (parentProperty == rbt_ERROR ||
        parentProperty == rbt_CORRUPTED)
    {
        return -1;
    }

    if (newNode->parent->redBlackFlag == rbt_RED)
    {
        rbt_node_t *uncle = getUncle(*head, newNode);
        if (uncle == NULL ||
            uncle->redBlackFlag == rbt_BLACK)
        {
            rbt_node_t *grandParent = newNode->parent->parent;
            if (newNodeProperty == rbt_LEFTCHILD &&
                parentProperty == rbt_LEFTCHILD)
            {
                swapNodeColor(newNode->parent, grandParent);
                rightRotateNode(head, grandParent);
                return 0;
            }
            else if (newNodeProperty == rbt_RIGHTCHILD &&
                     parentProperty == rbt_LEFTCHILD)
            {
                leftRotateNode(head, newNode->parent);
                swapNodeColor(newNode, grandParent);
                rightRotateNode(head, grandParent);
                return 0;
            }
            else if (newNodeProperty == rbt_RIGHTCHILD &&
                     parentProperty == rbt_RIGHTCHILD)
            {
                swapNodeColor(newNode->parent, grandParent);
                leftRotateNode(head, grandParent);
                return 0;
            }
            else if (newNodeProperty == rbt_LEFTCHILD &&
                     parentProperty == rbt_RIGHTCHILD)
            {
                rightRotateNode(head, newNode->parent);
                swapNodeColor(newNode, grandParent);
                leftRotateNode(head, grandParent);
                return 0;
            }
        }
        else if (uncle->redBlackFlag == rbt_RED)
        {
            newNode->parent->redBlackFlag = rbt_BLACK;
            uncle->redBlackFlag = rbt_BLACK;
            newNode->parent->parent->redBlackFlag = rbt_RED;
            return rbt_insertBalance(head, newNode->parent->parent);
        }
    }
    return -1;
}

rbt_node_t *rbt_findNode(
    rbt_node_t *head,
    unsigned int value,
    bool exactMatch,
    bool findUpperNearest)
{
    rbt_nodeProperty_t headProperty =
        rbt_getNodeProperty(head, head);
    if (headProperty != rbt_ROOT)
        return NULL;

    rbt_node_t *result = NULL;
    if (findUpperNearest)
        result = rbt_findUpperNearestNode(head, head, value);
    else
        result = rbt_findLowerNearestNode(head, head, value);

    if (exactMatch)
    {
        if (result->value == value)
            return result;
        else
            return NULL;
    }
    else
        return result;
    return NULL;
}

rbt_node_t *rbt_findUpperNearestNode(
    rbt_node_t *head,
    rbt_node_t *currentHead,
    unsigned int value)
{
    rbt_nodeProperty_t currentHeadProperty =
        rbt_getNodeProperty(head, currentHead);
    if (currentHeadProperty == rbt_ERROR ||
        currentHeadProperty == rbt_CORRUPTED)
        return NULL;
    if (currentHead->value == value)
        return currentHead;
    else if (currentHead->value > value)
    {
        if (currentHead->leftChild == NULL)
        {
            return currentHead;
        }
        return rbt_findUpperNearestNode(
            head,
            currentHead->leftChild,
            value);
    }
    else if (currentHead->value < value)
    {
        if (currentHead->rightChild == NULL)
        {
            rbt_node_t *me = currentHead;
            rbt_nodeProperty_t meProperty = currentHeadProperty;
            while (meProperty != rbt_LEFTCHILD)
            {
                if (meProperty == rbt_RIGHTCHILD)
                {
                    me = me->parent;
                    meProperty = rbt_getNodeProperty(head, me);
                }
                else
                {
                    return NULL;
                }
            }
            return me->parent;
        }
        return rbt_findUpperNearestNode(
            head,
            currentHead->rightChild,
            value);
    }
    else
        return NULL;
}

rbt_node_t *rbt_findLowerNearestNode(
    rbt_node_t *head,
    rbt_node_t *currentHead,
    unsigned int value)
{
    rbt_nodeProperty_t currentHeadProperty =
        rbt_getNodeProperty(head, currentHead);
    if (currentHeadProperty == rbt_ERROR ||
        currentHeadProperty == rbt_CORRUPTED)
        return NULL;
    if (currentHead->value == value)
        return currentHead;
    else if (currentHead->value > value)
    {
        if (currentHead->leftChild == NULL)
        {
            rbt_node_t *me = currentHead;
            rbt_nodeProperty_t meProperty = currentHeadProperty;
            while (meProperty != rbt_RIGHTCHILD)
            {
                if (meProperty == rbt_LEFTCHILD)
                {
                    me = me->parent;
                    break;
                }
                else
                {
                    return NULL;
                }
            }
            return me->parent;
        }
        return rbt_findUpperNearestNode(
            head,
            currentHead->leftChild,
            value);
    }
    else if (currentHead->value < value)
    {
        if (currentHead->rightChild == NULL)
        {
            return currentHead;
        }
        return rbt_findUpperNearestNode(
            head,
            currentHead->rightChild,
            value);
    }
    else
        return NULL;
}

int rbt_removeNode(
    rbt_node_t **head,
    rbt_node_t *target)
{
    rbt_nodeProperty_t targetProperty =
        rbt_getNodeProperty(*head, target);
    if (targetProperty == rbt_ERROR ||
        targetProperty == rbt_CORRUPTED)
    {
        return -1;
    }

    if (target->leftChild != NULL &&
        target->rightChild != NULL)
    {
        rbt_node_t *immediateSuccessor = target->rightChild;
        while (immediateSuccessor->leftChild != NULL)
        {
            immediateSuccessor = immediateSuccessor->leftChild;
        }
        swapNodeColor(target, immediateSuccessor);
        swapNode(head, target, immediateSuccessor);
        targetProperty = rbt_getNodeProperty(*head, target);
        if (targetProperty == rbt_ERROR ||
            targetProperty == rbt_CORRUPTED)
        {
            return -1;
        }
    }

    rbt_removeNodeBalance(head, target);

    if (target->leftChild == NULL &&
        target->rightChild == NULL)
    {
        switch (targetProperty)
        {
        case rbt_ROOT:
            *head = NULL;
            break;
        case rbt_LEFTCHILD:
            target->parent->leftChild = NULL;
            break;
        case rbt_RIGHTCHILD:
            target->parent->rightChild = NULL;
            break;
        default:
            return -1;
        }
    }
    else if (target->leftChild == NULL)
    {
        if (targetProperty == rbt_LEFTCHILD)
        {
            target->parent->leftChild = target->rightChild;
            target->rightChild->parent = target->parent;
        }
        else if (targetProperty == rbt_RIGHTCHILD)
        {
            target->parent->rightChild = target->rightChild;
            target->rightChild->parent = target->parent;
        }
        else
            return -1;
    }
    else if (target->rightChild == NULL)
    {
        if (targetProperty == rbt_LEFTCHILD)
        {
            target->parent->leftChild = target->leftChild;
            target->leftChild->parent = target->parent;
        }
        else if (targetProperty == rbt_RIGHTCHILD)
        {
            target->parent->rightChild = target->leftChild;
            target->leftChild->parent = target->parent;
        }
        else
            return -1;
    }
    else
    {
        return -1;
    }

    return 0;
}

int rbt_removeNodeBalance(
    rbt_node_t **head,
    rbt_node_t *target)
{
    rbt_nodeProperty_t targetProperty =
        rbt_getNodeProperty(*head, target);
    if (getNodeColor(target) == rbt_RED ||
        getNodeColor(target->leftChild) == rbt_RED ||
        getNodeColor(target->rightChild) == rbt_RED)
    {
        if (target->leftChild == NULL &&
            target->rightChild == NULL)
        {
            return 0;
        }
        else if (target->leftChild == NULL)
        {
            target->rightChild->redBlackFlag = rbt_BLACK;
            return 0;
        }
        else if (target->rightChild == NULL)
        {
            target->leftChild->redBlackFlag = rbt_RED;
            return 0;
        }
        else
            return -1;
    }
    else if (targetProperty == rbt_ROOT)
    {
        if (target->leftChild != NULL)
            target->leftChild->redBlackFlag = rbt_BLACK;
        else if (target->rightChild != NULL)
            target->rightChild->redBlackFlag = rbt_BLACK;
        return 0;
    }
    else if (getNodeColor(target) == rbt_BLACK &&
             getNodeColor(target->leftChild) == rbt_BLACK &&
             getNodeColor(target->rightChild) == rbt_BLACK)
    {
        rbt_node_t *sibling = getSibling(*head, target);
        rbt_node_t *parent = sibling->parent;
        rbt_nodeProperty_t siblingProperty =
            rbt_getNodeProperty(*head, sibling);

        if (getNodeColor(sibling) == rbt_BLACK &&
            sibling != NULL &&
            (getNodeColor(sibling->leftChild) == rbt_RED ||
             getNodeColor(sibling->rightChild) == rbt_RED))
        {
            if (siblingProperty == rbt_LEFTCHILD &&
                getNodeColor(sibling->leftChild) == rbt_RED)
            {
                swapNodeColor(sibling, parent);
                sibling->leftChild->redBlackFlag = rbt_BLACK;
                rightRotateNode(head, parent);
                return 0;
            }
            else if (siblingProperty == rbt_LEFTCHILD &&
                     getNodeColor(sibling->rightChild) == rbt_RED)
            {
                swapNodeColor(sibling, sibling->rightChild);
                leftRotateNode(head, sibling);
                rbt_removeNodeBalance(head, target);
                return 0;
            }
            else if (siblingProperty == rbt_RIGHTCHILD &&
                     getNodeColor(sibling->rightChild) == rbt_RED)
            {
                swapNodeColor(sibling, parent);
                sibling->rightChild->redBlackFlag = rbt_BLACK;
                leftRotateNode(head, parent);
                return 0;
            }
            else if (siblingProperty == rbt_RIGHTCHILD &&
                     getNodeColor(sibling->leftChild) == rbt_RED)
            {
                swapNodeColor(sibling, sibling->leftChild);
                rightRotateNode(head, sibling);
                rbt_removeNodeBalance(head, target);
                return 0;
            }
            else
                return -1;
        }
        else if (sibling == NULL ||
                 (getNodeColor(sibling) == rbt_BLACK &&
                  getNodeColor(sibling->leftChild) == rbt_BLACK &&
                  getNodeColor(sibling->rightChild) == rbt_BLACK))
        {
            if (sibling != NULL)
            {
                sibling->redBlackFlag = rbt_RED;
                if (sibling->parent->redBlackFlag == rbt_RED)
                {
                    sibling->parent->redBlackFlag = rbt_BLACK;
                }
                else if (sibling->parent->redBlackFlag == rbt_BLACK)
                    rbt_removeNodeBalance(head, sibling->parent);
                else
                    return -1;
                return 0;
            }
            else
                return -1;
        }
        else if (sibling->redBlackFlag == rbt_RED)
        {
            swapNodeColor(sibling, parent);
            if (siblingProperty == rbt_RIGHTCHILD)
            {
                sibling = sibling->leftChild;
                leftRotateNode(head, parent);
                rbt_removeNodeBalance(head, target);
            }
            else if (siblingProperty == rbt_LEFTCHILD)
            {
                sibling = sibling->rightChild;
                rightRotateNode(head, parent);
                rbt_removeNodeBalance(head, target);
            }
            else
            {
                return -1;
            }
            return 0;
        }
    }
    return -1;
}