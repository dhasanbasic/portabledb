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
	node->meta = nodemeta;
	node->data = (char*)malloc(nodemeta->nodeLength);
	return node;
}

void	WriteNode(FILE* file, BtNode* node, const long int position)
{
	fseek(file, position, SEEK_SET);
	fwrite(node->data, node->meta->nodeLength, 1, file);
}

void	ReadNode(FILE* file, BtNode* node, const long int position)
{
	fseek(file, position, SEEK_SET);
	fread(node->data, node->meta->nodeLength, 1, file);
}
