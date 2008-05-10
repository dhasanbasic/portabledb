
#include "node.h"

#include <malloc.h>

/**
 * Allocates a B-tree structure in memory
 * 
 * @param order			order of the B-tree
 * @param recordSize	length of a record
 * @param key			position of the key of a record
 * @param keySize		length of the key of a record
 */
BTREE* allocateBtree(INT order, INT recordSize, INT key, INT keySize)
{
	BTREE* btree = (BTREE*)malloc(sizeof(BTREE));
	
	btree->order = order;
	btree->recordSize = recordSize;
	btree->key = key;
	btree->keySize = keySize;
	btree->root = 0;
	
	return btree;
}

/**
 * Frees the memory allocated by a B-tree structure
 */
void freeBtree(BTREE* btree)
{
	free(btree);
}
