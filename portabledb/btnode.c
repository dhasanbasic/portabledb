/*
 * btnode.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "btree.h"

#include <malloc.h>
#include <stdio.h>

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
