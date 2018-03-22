#ifndef RBT_RBTHELPER_H_
#define RBT_RBTHELPER_H_

#include <kernel/datalayer/allocator/redBlackTree/rbt.h>

int leftRotateNode(rbt_node_t **head, rbt_node_t *target);
int rightRotateNode(rbt_node_t **head, rbt_node_t *target);
int swapNode(rbt_node_t **head, rbt_node_t *a, rbt_node_t *b);
int swapNodeColor(rbt_node_t *a, rbt_node_t *b);
rbt_node_t *getUncle(rbt_node_t *head, rbt_node_t *target);
rbt_node_t *getSibling(rbt_node_t *head, rbt_node_t *target);
rbt_redBlackFlag_t getNodeColor(rbt_node_t *target);

void printTree(rbt_node_t *head, unsigned int level);

#endif