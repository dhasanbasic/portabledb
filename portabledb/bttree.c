/*
 * btree.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "btree.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

BtTree*	CreateTree(
			FILE*		file,
			const SHORT	order,
			const SHORT	recordLength,
			const SHORT	keyPosition,
			const SHORT	keyLength,
			const SHORT	freelistSize)
{
	BtTree* tree;
	BtNode* root;

	/* allocate the meta-data structures */
	tree = (BtTree*)malloc(sizeof(BtTree));
	tree->meta = (BtMeta*)malloc(sizeof(BtMeta));

	/* fill in the B-tree meta-data */
	tree->meta->order			= order;
	tree->meta->recordLength	= recordLength;
	tree->meta->keyPosition		= keyPosition;
	tree->meta->keyLength		= keyLength;
	tree->meta->freelistSize	= freelistSize;
	tree->meta->freeNodes		= 0;

	/* fill in the node meta-data */
	CalculateNodeMeta(tree);
	tree->freelist = (LONG*)malloc(freelistSize * sizeof(LONG));

	/* allocate the root node */
	root = AllocateNode(tree,MODE_MEMORY);

	/* fill in the root node */
	SetCount(root, 0);
	SetLeaf(root, LEAF);

	/* determine the positions of the structures in the file */
	tree->file = file;
	fseek(file,0,SEEK_END);

	tree->position = ftell(file);
	tree->meta->freelistPosition = tree->position + sizeof(BtMeta);
	tree->meta->rootPosition = tree->position + sizeof(BtMeta)
		+ freelistSize*sizeof(LONG);
	root->position = tree->meta->rootPosition;

	/* save the structures */
	fwrite(tree->meta,sizeof(BtMeta),1,file);
	fwrite(tree->freelist, sizeof(LONG), freelistSize, file);
	fwrite(root->data, tree->nodeLength, 1, file);

	tree->root = root;

	return tree;
}


void	WriteTree(BtTree* tree)
{
	BtNode* root = (BtNode*)tree->root;

	fseek(tree->file, tree->position, SEEK_SET);
	fwrite(tree->meta,sizeof(BtMeta),1,tree->file);

	fseek(tree->file, tree->meta->freelistPosition, SEEK_SET);
	fwrite(tree->freelist, sizeof(LONG),
			tree->meta->freelistSize, tree->file);
}

void	ReadTree(BtTree* tree)
{
	BtNode* root;

	/* fill in the meta-data structures */
	tree->meta		= (BtMeta*)malloc(sizeof(BtMeta));
	fseek(tree->file, tree->position, SEEK_SET);
	fread(tree->meta,sizeof(BtMeta),1,tree->file);

	CalculateNodeMeta(tree);

	/* create and fill the free list */
	tree->freelist = (LONG*) malloc(tree->meta->freelistSize*sizeof(LONG));

	fseek(tree->file, tree->meta->freelistPosition, SEEK_SET);
	fread(tree->freelist, sizeof(LONG), tree->meta->freelistSize, tree->file);

	/* create and fill in the root node */
	root			= (BtNode*)malloc(sizeof(BtNode));
	root->data		= (char*)malloc(tree->nodeLength);
	root->tree		= tree;
	root->position	= tree->meta->rootPosition;

	fseek(tree->file, tree->meta->rootPosition, SEEK_SET);
	fread(root->data, tree->nodeLength, 1, tree->file);

	tree->root = root;
}

void	InsertRecord(BtTree* tree, const void* record)
{
	BtreeInsert(tree, record);
}

int		SearchRecord(BtTree* tree, void* key, void* record)
{
	BtSearchParam* p = (BtSearchParam*)malloc(sizeof(BtSearchParam));
	int result;

	p->node = (BtNode*)tree->root;
	p->key = (char*)key;
	p->result = SEARCH_NOTFOUND;

	BtreeSearch(p);

	if (p->result == SEARCH_FOUND)
		memcpy(record, GetRecord(p->node,p->index), tree->meta->recordLength);

	if (p->node->position != tree->meta->rootPosition)
	{
		free(p->node->data);
		free(p->node);
	}

	result = p->result;
	free(p);

	return result;
}
