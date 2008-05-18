
#include "btreelayer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* ************************************************************************** */
/* 							   MEMORY OPERATIONS							  */
/* ************************************************************************** */

BTREE*	createBtree(FILE*		file,
					const SINT	order,
					const SINT	recordSize,
					const SINT	keyPosition,
					const SINT	keySize)
{
	BTREE* btree 				= (BTREE*)malloc(sizeof(BTREE));
	btree->file 				= file;
	btree->position 			= 0;
	
	btree->tag					= (BTREETAG*)malloc(sizeof(BTREETAG));
	btree->tag->order			= order;
	btree->tag->recordSize		= recordSize;
	btree->tag->keyPosition		= keyPosition;
	btree->tag->keySize			= keySize;
	btree->tag->rootPosition	= 0;
	
	return btree;
}

void	freeBtree(BTREE* btree)
{
	free(btree->tag);
	free(btree);
}

NODE*	createNode(BTREE* btree)
{
	NODE* node			= (NODE*)malloc(sizeof(NODE));
	node->btree 		= btree;
	node->position		= 0;
	
	node->tag			= (NODETAG*)malloc(sizeof(NODETAG));
	node->tag->records 	= 0;
	node->tag->data		= (char*)malloc(MORDER(btree) * MRECSIZE(btree));
	node->tag->subnodes	= (LONG*)calloc(MORDER(btree)+1, sizeof(LONG));
	
	return node;
}

void	freeNode(NODE* node)
{
	cfree(node->tag->subnodes);
	free(node->tag->data);
	free(node);
}
/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							    FILE OPERATIONS								  */
/* ************************************************************************** */

void	loadBtree(BTREE* btree)
{
	fseek(btree->file, btree->position, SEEK_SET);
	fread(btree->tag,sizeof(BTREETAG),1,btree->file);
}

void	saveBtree(BTREE* btree)
{
	if(btree->position == 0)
	{
		fseek(btree->file, 0, SEEK_END);
		btree->position = ftell(btree->file);
	}
	else
		fseek(btree->file, btree->position, SEEK_SET);
	
	fwrite(btree->tag,sizeof(BTREETAG),1,btree->file);
}

void	loadNode(NODE* node)
{
	unsigned int numSize 	= sizeof(SINT);
	unsigned int datSize 	= MORDER(node->btree) * MRECSIZE(node->btree);
	unsigned int subSize 	= (MORDER(node->btree)+1) * sizeof(LONG);
	unsigned int buffSize	= numSize + datSize + subSize; 	
	
	char* buff = (char*)malloc(buffSize);
	
	memcpy(buff+0,				 &MRECORDS(node), numSize);
	memcpy(buff+numSize,		 MDATA(node), datSize);
	memcpy(buff+numSize+datSize, MSUBNODES(node), subSize);
	
	fseek(node->btree->file, node->position, SEEK_SET);
	fwrite(buff,buffSize,1,node->btree->file);
}

void	saveNode(NODE* node)
{
	unsigned int numSize 	= sizeof(SINT);
	unsigned int datSize 	= MORDER(node->btree) * MRECSIZE(node->btree);
	unsigned int subSize 	= (MORDER(node->btree)+1) * sizeof(LONG);
	unsigned int buffSize	= numSize + datSize + subSize; 	
	
	char* buff = (char*)malloc(buffSize);

	if(node->position == 0)
	{
		fseek(node->btree->file, 0, SEEK_END);
		node->position = ftell(node->btree->file);
	}
	else
		fseek(node->btree->file, node->position, SEEK_SET);
	
	fread(buff,buffSize,1,node->btree->file);
	
	memcpy(&MRECORDS(node),	buff+0, numSize);
	memcpy(MDATA(node),		buff+numSize, datSize);
	memcpy(MSUBNODES(node),	buff+numSize+datSize, subSize);
}

/* ********************************** *** *********************************** */
