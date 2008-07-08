/*
 * btnode.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "../btree.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

BtNode* AllocateNode(BtTree* tree, const int mode)
{
	BtNode* node = (BtNode*)malloc(sizeof(BtNode));

	node->data = (char*)malloc(tree->nodeLength);
	node->tree = tree;
	node->position = 0;

	if(mode == MODE_FREELIST)
	{
		if(tree->meta->freeNodes > 0)
		{
			node->position = tree->freelist[tree->meta->freeNodes - 1];
			tree->meta->freeNodes--;
		}
	}

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
