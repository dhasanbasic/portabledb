#include "node.h"

#include <malloc.h>
#include <string.h>
#include <stdio.h>

/**
 * Allocates a B-tree structure in memory
 * 
 * @param order			order of the B-tree
 * @param recordSize	length of a record
 * @param key			position of the key of a record
 * @param keySize		length of the key of a record
 */
BTREE* allocateBtree(INT order, INT recordSize, INT key, INT keySize) {
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
void freeBtree(BTREE* btree) {
	free(btree);
}

/**
 * Searches for a record in a specific B-tree
 */
void searchRecord(SEARCHPARAM* p) {
	
	INT keyLeft, keyRight, i;

	keyLeft = p->btree->key;
	keyRight = p->node->recordCount * p->btree->recordSize + p->btree->key;
	
	/* left subnode? */
	if (memcmp(p->key, &p->node->data[keyLeft], p->btree->keySize) < 0)
		p->subnode = 0;
	
	/* right subnode? */
	if (memcmp(p->key, &p->node->data[keyRight], p->btree->keySize) > 0)
		p->subnode = p->node->recordCount + 1;
	
	if (p->node->subnodes[p->subnode] == 0)
	{
		p->wasFound = -1;
		return;
	}
	else
	{
		freeNode(p->node);
		p->node = loadNode(p->btree, p->file, p->node->subnodes[p->subnode]);
		searchRecord(p);
	}
	
	/* inside of the node? */
	for (i = 0; i < (p->node->recordCount-1); i++)
	{
		keyLeft = p->node->recordCount * i + p->btree->key;
		keyRight = p->node->recordCount * (i+1) + p->btree->key;
		
		/* at i-th position? */
		if(memcmp(p->key, &p->node->data[keyLeft], p->btree->keySize) == 0)
		{
			p->position = i;
			p->wasFound = 1;
			return;
		}
		
		/* between record(i) and record(i+1) ? */
		if( memcmp(p->key, &p->node->data[keyLeft], p->btree->keySize) > 0 &&
		   memcmp(p->key, &p->node->data[keyRight], p->btree->keySize) < 0)
		{
			p->subnode = i+1;
			
			if (p->node->subnodes[p->subnode] == 0)
			{
				p->wasFound = -1;
				return;
			}
			else
			{
				freeNode(p->node);
				p->node = loadNode(p->btree, p->file, p->node->subnodes[p->subnode]);
				searchRecord(p);
			}			
		}
	}
}

/**
 * Inserts a record in a specific B-tree
 */
void insertRecord(FILE* file, BTREE* btree, NODE* root, void* record) {
	
	SEARCHPARAM p;
	
	p.file = file;
	p.btree = btree;
	p.key = (char*)&record[btree->key];
	
}
