
#include "node.h"

#include <malloc.h>
#include <string.h>

/* ****************************************************************************
 *      Allocates a node for a specific B-tree
 * ****************************************************************************/

NODE* allocateNode(BTREE* btree)
{
    NODE* node = (NODE*)malloc(sizeof(NODE));

    node->recordCount = 0;
    node->subnodeLeft = 0;
    node->data = (char*)malloc(btree->order * btree->recordSize);
    node->subnodes = (LONG*)calloc(btree->order,sizeof(LONG));

    return node;
}

/* ****************************************************************************
 *      Frees the memory used by a node
 * ****************************************************************************/

void freeNode(NODE* node)
{
    cfree(node->subnodes);
    free(node->data);
    free(node);
}
