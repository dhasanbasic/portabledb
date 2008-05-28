
#include "types.h"

#include <malloc.h>

/******************************************************************************
 * 					   		  ALLOCATION, DEALLOCATION					  	  *
 ******************************************************************************/

void NodeInit(BNODE* node)
{
	node->leaf			= node->data;
	node->recordCount	= node->leaf + sizeof(SHORT);
	node->records		= node->recordCount + sizeof(SHORT);
	node->children		= RECORD_PTR(node,RMAX(node)) + RLEN(node); 
}

BNODE* NodeAllocate(BTREE* btree)
{
	BNODE* node = (BNODE*)malloc(sizeof(BNODE));
	
	node->meta		= btree->meta;
	node->file		= btree->file;
	
	node->data = (char*)malloc(btree->meta->nodeLength);
	
	NodeInit(node);
	
	return node;
}

void NodeFree(BNODE* node)
{
	free(node->data);
	free(node);
}

BTREE* TreeCreate(FILE* file, SHORT degree, SHORT recordLength,
		          SHORT keyPosition, SHORT keyLength)
{	
	BTREE* btree 	= (BTREE*)malloc(sizeof(BTREE));
	btree->meta		= (BTREEMETA*)malloc(sizeof(BTREEMETA));
	
	btree->file = file;
	
	fseek(file, 0, SEEK_END);
	btree->position = ftell(file);
	
	btree->meta->degree 		= degree;
	btree->meta->recordLength	= recordLength;
	btree->meta->keyPosition	= keyPosition;
	btree->meta->keyLength		= keyLength;
	btree->meta->minRecords		= degree - 1;
	btree->meta->maxRecords		= 2*degree - 1;
	btree->meta->rootPosition	= btree->position + sizeof(BTREEMETA);
	
	btree->meta->nodeLength		=
		  2 * sizeof(SHORT)
		+ btree->meta->maxRecords * btree->meta->recordLength
		+ (btree->meta->maxRecords+1) * sizeof(LONG);
	
	btree->root = NodeAllocate(btree);
	LEAF(btree->root) = 1;
	btree->root->position = btree->meta->rootPosition;
	
	return btree;
}

void TreeFree(BTREE* btree)
{
	NodeFree(btree->root);
	free(btree->meta);
	free(btree);
}
