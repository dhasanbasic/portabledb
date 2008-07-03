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
	root = AllocateNode(tree);

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

	fseek(tree->file, tree->meta->rootPosition, SEEK_SET);
	fwrite(root->data, tree->nodeLength, 1, tree->file);
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

void	SearchTree(BtSearchParam* p)
{
	SHORT i = 1;
	SHORT keyLength = p->node->tree->meta->keyLength;

	while (i <= GetCount(p->node)
		&& (memcmp(p->key, GetKey(p->node,i), keyLength) > 0)) i++;

	if (i <= GetCount(p->node)
		&& (memcmp(p->key, GetKey(p->node,i), keyLength) == 0))
	{
		p->result = SEARCH_FOUND;
		p->index = i;
		return;
	}

	if (GetLeaf(p->node) == LEAF)
	{
		p->result = SEARCH_NOTFOUND;
		return;
	}
	else
	{
		if(p->node == (BtNode*)p->node->tree->root)
		{
			BtTree* tree = p->node->tree;
			p->node = (BtNode*)malloc(sizeof(BtNode));
			p->node->tree = tree;
		}

		p->node->position = *((LONG*)GetChild(p->node,i));
		ReadNode(p->node);
		SearchTree(p);
	}
}
