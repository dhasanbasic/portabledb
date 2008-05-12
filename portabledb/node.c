
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
    node->data = (char*)malloc(btree->order * btree->recordSize);
    node->subnodes = (LONG*)calloc(btree->order+1,sizeof(LONG));

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

/* ****************************************************************************
 *      Loads a specific node from a B-tree
 * ****************************************************************************/
NODE* loadNode(BTREE* btree, FILE* file, LONG position)
{
	NODE* node = allocateNode(btree);
	
	fseek(file, position, SEEK_SET);
	
	fread(node->recordCount, sizeof(INT), 1, file);
	fread(node->data, sizeof(char), btree->order * btree->recordSize, file);
	fread(node->subnodes, sizeof(LONG), btree->order + 1, file);
	
	return node;
}