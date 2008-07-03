/*
 * btnode.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "btree.h"

#include <malloc.h>
#include <stdio.h>

BtNode* AllocateNode(BtNodeMeta* nodemeta)
{
	BtNode* node = (BtNode*)malloc(sizeof(BtNode));

	node->data = (char*)malloc(nodemeta->nodeLength);

	/* is there any free node? */
	if(nodemeta->freeNodes > 0)
	{
		node->position = nodemeta->freelist[nodemeta->freeNodes - 1];
		nodemeta->freeNodes--;
	}
	else
		node->position = 0;

	return node;
}

void	WriteNode(FILE* file, const BtNodeMeta* nodemeta, BtNode* node)
{
	if(node->position > 0)
		fseek(file, node->position, SEEK_SET);
	else
	{
		fseek(file, 0, SEEK_END);
		node->position = ftell(file);
	}
	fwrite(node->data, nodemeta->nodeLength, 1, file);
}

void	ReadNode(FILE* file, const BtNodeMeta* nodemeta, BtNode* node)
{
	fseek(file, node->position, SEEK_SET);
	fread(node->data, nodemeta->nodeLength, 1, file);
}
