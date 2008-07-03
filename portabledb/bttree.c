/*
 * btree.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "btree.h"

#include <malloc.h>
#include <stdio.h>

BtTree*	CreateTree(
			FILE*		file,
			const SHORT	order,
			const SHORT	recordLength,
			const SHORT	keyPosition,
			const SHORT	keyLength,
			const SHORT	freelistSize)
{
	BtTree*  tree;

	/* allocate the meta-data structures */
	tree = (BtTree*)malloc(sizeof(BtTree));
	tree->meta = (BtMeta*)malloc(sizeof(BtMeta));
	tree->nodemeta = (BtNodeMeta*)malloc(sizeof(BtNodeMeta));

	/* fill in the B-tree meta-data */
	tree->meta->order			= order;
	tree->meta->recordLength	= recordLength;
	tree->meta->keyPosition		= keyPosition;
	tree->meta->keyLength		= keyLength;
	tree->meta->freelistSize	= freelistSize;
	tree->meta->freeNodes		= 0;

	/* fill in the node meta-data */
	CalculateNodeMeta(tree->meta, tree->nodemeta);
	tree->nodemeta->freeNodes	= 0;
	tree->nodemeta->freelistSize = freelistSize;
	tree->nodemeta->freelist = (LONG*)malloc(freelistSize * sizeof(LONG));

	/* allocate the root node */
	tree->root = AllocateNode(tree->nodemeta);

	/* fill in the root node */
	SetCount(tree->nodemeta, tree->root, 0);
	SetLeaf(tree->nodemeta, tree->root, LEAF);

	/* determine the positions of the structures in the file */
	tree->file = file;
	fseek(file,0,SEEK_END);

	tree->position = ftell(file);
	tree->meta->freelistPosition = tree->position + sizeof(BtMeta);
	tree->meta->rootPosition = tree->position + sizeof(BtMeta)
		+ freelistSize*sizeof(LONG);

	/* save the structures */
	fwrite(tree->meta,sizeof(BtMeta),1,file);
	fwrite(tree->nodemeta->freelist, sizeof(LONG), freelistSize, file);
	fwrite(tree->root->data, tree->nodemeta->nodeLength, 1, file);

	return tree;
}


void	WriteTree(BtTree* tree)
{
	tree->meta->freeNodes = tree->nodemeta->freeNodes;

	fseek(tree->file, tree->position, SEEK_SET);
	fwrite(tree->meta,sizeof(BtMeta),1,tree->file);

	fseek(tree->file, tree->meta->freelistPosition, SEEK_SET);
	fwrite(tree->nodemeta->freelist, sizeof(LONG),
			tree->meta->freelistSize, tree->file);

	fseek(tree->file, tree->meta->rootPosition, SEEK_SET);
	fwrite(tree->root->data, tree->nodemeta->nodeLength, 1, tree->file);
}

void	ReadTree(BtTree* tree)
{
	/* allocate the meta-data structures */
	tree->meta		= (BtMeta*)malloc(sizeof(BtMeta));
	tree->nodemeta	= (BtNodeMeta*)malloc(sizeof(BtNodeMeta));

	/* fill in the meta-data structures */
	fseek(tree->file, tree->position, SEEK_SET);
	fread(tree->meta,sizeof(BtMeta),1,tree->file);

	CalculateNodeMeta(tree->meta, tree->nodemeta);

	/* create and fill the free list */
	tree->nodemeta->freeNodes 	 = tree->meta->freeNodes;
	tree->nodemeta->freelistSize = tree->meta->freelistSize;
	tree->nodemeta->freelist	 =
		(LONG*) malloc(tree->meta->freelistSize*sizeof(LONG));

	fseek(tree->file, tree->meta->freelistPosition, SEEK_SET);
	fread(tree->nodemeta->freelist, sizeof(LONG),
			tree->meta->freelistSize, tree->file);

	/* create and fill in the root node */
	tree->root			 = (BtNode*)malloc(sizeof(BtNode));
	tree->root->data	 = (char*)malloc(tree->nodemeta->nodeLength);
	tree->root->position = tree->meta->rootPosition;

	fseek(tree->file, tree->meta->rootPosition, SEEK_SET);
	fread(tree->root->data, tree->nodemeta->nodeLength, 1, tree->file);
}
