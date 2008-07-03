/*
 * btnode.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "btree.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

BtNode* AllocateNode(BtTree* tree)
{
	BtNode* node = (BtNode*)malloc(sizeof(BtNode));

	node->data = (char*)malloc(tree->nodeLength);
	node->tree = tree;

	/* is there any free node? */
	if(tree->meta->freeNodes > 0)
	{
		node->position = tree->freelist[tree->meta->freeNodes - 1];
		tree->meta->freeNodes--;
	}
	else
		node->position = 0;

	return node;
}

void	WriteNode(BtNode* node)
{
	if(node->position > 0)
		fseek(node->tree->file, node->position, SEEK_SET);
	else
	{
		fseek(node->tree->file, 0, SEEK_END);
		node->position = ftell(node->tree->file);
	}
	fwrite(node->data, node->tree->nodeLength, 1, node->tree->file);
}

void	ReadNode(BtNode* node)
{
	fseek(node->tree->file, node->position, SEEK_SET);
	fread(node->data, node->tree->nodeLength, 1, node->tree->file);
}

void	SplitChild(
			BtNode* x,
			const SHORT i,
			BtNode* y)
{
	BtTree* tree		= x->tree;
	BtNode* z			= AllocateNode(tree);
	SHORT order			= tree->meta->order;
	SHORT recordLength	= tree->meta->recordLength;
	SHORT minRecords	= tree->minRecords;

	/* update the new child node (z) */
	SetLeaf(z, GetLeaf(y));
	SetCount(z, minRecords);

	memcpy(GetRecord(z,1), GetRecord(y,order+1), minRecords * recordLength);

	if (GetLeaf(y) == INTERNAL)
	{
		memcpy(GetChild(z,1), GetChild(y,order+1), order * sizeof(LONG));
	}

//	WriteNode(z);

	/* update the split child node */
	SetCount(y, minRecords);
//	WriteNode(y);

	/* update the parent node */
	memmove(GetChild(x,i+2), GetChild(x,i+1), (GetCount(x)-i+1)*sizeof(LONG));
	memmove(GetRecord(x,i+1), GetRecord(x,i), (GetCount(x)-i+1)*recordLength);

	*((LONG*)GetChild(x,i+1)) = z->position;
	memcpy(GetRecord(x,i), GetRecord(y,order), recordLength);

	SetCount(x, GetCount(x)+1);

//	WriteNode(x);
}

