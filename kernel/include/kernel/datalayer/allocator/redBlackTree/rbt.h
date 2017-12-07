#ifndef RBT_RBT_H_
#define RBT_RBT_H_

#include <kernel/kdef.h>
#include <stdbool.h>

enum rbt_redBlackFlag
{
    rbt_RED,
    rbt_BLACK
};
enum rbt_nodeProperty
{
    rbt_ROOT,
    rbt_LEFTCHILD,
    rbt_RIGHTCHILD,
    rbt_CORRUPTED,
    rbt_ERROR
};
typedef enum rbt_redBlackFlag rbt_redBlackFlag_t;
typedef enum rbt_nodeProperty rbt_nodeProperty_t;

struct rbt_node;
typedef struct rbt_node rbt_node_t;

struct rbt_node
{
    rbt_node_t *parent;
    rbt_node_t *leftChild;
    rbt_node_t *rightChild;
    rbt_redBlackFlag_t redBlackFlag;
    uint32_t value;
    nvmos_pointer_t content;
};

rbt_nodeProperty_t rbt_getNodeProperty(
    rbt_node_t *head,
    rbt_node_t *target);

int rbt_insertNode(
    rbt_node_t **head,
    rbt_node_t *newNode);

int rbt_insertBalance(
    rbt_node_t **head,
    rbt_node_t *newNode);

rbt_node_t *rbt_findNode(
    rbt_node_t *head,
    unsigned int value,
    bool exactMatch,
    bool findUpperNearest);
rbt_node_t *rbt_findUpperNearestNode(
    rbt_node_t *head,
    rbt_node_t *currentHead,
    unsigned int value);
rbt_node_t *rbt_findLowerNearestNode(
    rbt_node_t *head,
    rbt_node_t *currentHead,
    unsigned int value);

int rbt_removeNode(
    rbt_node_t **head,
    rbt_node_t *target);
int rbt_removeNodeBalance(
    rbt_node_t **head,
    rbt_node_t *target);
int rbt_removeNodeUnlink(
    rbt_node_t **head,
    rbt_node_t *target);

#endif