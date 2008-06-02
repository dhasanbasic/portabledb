
#include "btreelayer.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>


void fillDescriptor(NODEDESC* desc)
{
	desc->minRecords		= desc->btreedesc->minChildren - 1;
	desc->maxRecords		= desc->btreedesc->minChildren*2 - 1;
	desc->recordsOffset		= 0;
	desc->childrenOffset	= desc->maxRecords * desc->btreedesc->recordSize;
	desc->countOffset		= desc->childrenOffset
							+ (desc->maxRecords+1) * sizeof(LONG);
	desc->leafOffset		= desc->countOffset + sizeof(SHORT);
	desc->nodeSize			= desc->leafOffset + sizeof(SHORT);
}

BTREE* readBtree(FILE* file, LONG position)
{
	BTREE* btree = (BTREE*)malloc(sizeof(BTREE));	
	btree->desc				= (NODEDESC*)malloc(sizeof(NODEDESC));
	btree->desc->btreedesc	= (BTREEDESC*)malloc(sizeof(BTREEDESC));
	
	fseek(file, position, SEEK_SET);
	fread(btree->desc->btreedesc, sizeof(BTREEDESC), 1, file);
	btree->position = position;
	
	fillDescriptor(btree->desc);
	
	btree->root = allocNode(btree->desc);
	readNode(file, btree->desc->btreedesc->rootPtr, btree->root);
	
	return btree;
}

void writeBtree(FILE* file, BTREE* btree)
{
	if(btree->position == 0)
	{
		fseek(file, 0, SEEK_END);
		btree->position = ftell(file);		
	}
	else
		fseek(file, btree->position, SEEK_SET);
	
	fwrite(btree->desc->btreedesc, sizeof(BTREEDESC), 1, file);
}

BTREE* createBtree(FILE* file, BTREEDESC* btreedesc)
{
	BTREE* btree = (BTREE*)malloc(sizeof(BTREE));	
	btree->desc				= (NODEDESC*)malloc(sizeof(NODEDESC));
	btree->desc->btreedesc	= (BTREEDESC*)malloc(sizeof(BTREEDESC));
	
	memcpy(btree->desc->btreedesc, btreedesc, sizeof(BTREEDESC));
	
	// calculate the node descriptor data
	fillDescriptor(btree->desc);
	
	// write the B-tree (first pass)
	writeBtree(file, btree);
	
	// create the root node
	btree->root = allocNode(btree->desc);
	setLeaf(btree->root,1);
	btree->desc->btreedesc->rootPtr = writeNode(file, 0, btree->root);
	
	writeBtree(file, btree);
	
	return btree;
}
