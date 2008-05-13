
#include "node.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

NODE* createNode(BTREE* btree)
{
	NODE* node = (NODE*)malloc(sizeof(NODE));
	
	node->btree = btree;
	node->position = 0;
	node->records = 0;
	node->data = (char*)malloc(btree->tag->rSize * btree->tag->order);
	node->subnodes = (LONG*)calloc(btree->tag->order+1, sizeof(LONG));
	
	return node;
}

void freeNode(NODE* node)
{
	cfree(node->subnodes);
	free(node->data);
	free(node);
}

void loadNode(NODE* node)
{
	UINT posData = sizeof(UINT);
	UINT sizeData = node->btree->tag->order * node->btree->tag->rSize;
	UINT posSubnodes = posData + sizeData;
	UINT sizeSubnodes = (node->btree->tag->order + 1) * sizeof(LONG);
	
	UINT sizeBuffer = posSubnodes + sizeSubnodes;
	
	char* buffer = (char*)malloc(sizeBuffer);

	fseek(node->btree->file, node->position, SEEK_SET);
	fread(buffer, sizeBuffer, 1, node->btree->file);
		
	memcpy(&node->records, &buffer[0], posData);
	memcpy(node->data, &buffer[posData], sizeData);
	memcpy(node->subnodes, &buffer[posSubnodes], sizeSubnodes);
	
	free(buffer);
}

void saveNode(NODE* node)
{
	UINT posData = sizeof(UINT);
	UINT sizeData = node->btree->tag->order * node->btree->tag->rSize;
	UINT posSubnodes = posData + sizeData;
	UINT sizeSubnodes = (node->btree->tag->order + 1) * sizeof(LONG);
	
	UINT sizeBuffer = posSubnodes + sizeSubnodes;
	
	char* buffer = (char*)malloc(sizeBuffer);
		
	memcpy(&buffer[0], &node->records, posData);
	memcpy(&buffer[posData], node->data, sizeData);
	memcpy(&buffer[posSubnodes], node->subnodes, sizeSubnodes);

	fseek(node->btree->file, node->position, SEEK_SET);
	fwrite(buffer, sizeBuffer, 1, node->btree->file);
	
	free(buffer);
}
