
#include "btreelayer.h"
#include <malloc.h>

void freeBtree(BTREE* btree)
{
	free(btree->desc->btreedesc);
	free(btree->desc);
	if(btree->root > 0) freeNode(btree->root);
	free(btree);
}

NODE* allocNode(NODEDESC* desc)
{
	NODE* node = (NODE*)malloc(sizeof(NODE));	
	node->desc = desc;
	node->data = (char*)malloc(desc->nodeSize);
	
	return node;
}

void freeNode(NODE* node)
{
	free(node->data);
	free(node);
}
