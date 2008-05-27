
#include "btreelayer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* ************************************************************************** */
/* 							   MEMORY OPERATIONS							  */
/* ************************************************************************** */

void	createBtree(BTREE*		btree,
					FILE*		file,
					const SHORT	degree,
					const SHORT	recordLen,
					const SHORT	keyPos,
					const SHORT	keyLen)
{
	btree->file 			= file;
	
	fseek(file, 0, SEEK_END);
	
	btree->position 		= ftell(file);
	
	btree->data->degree		= degree;
	btree->data->recordLen	= recordLen;
	btree->data->keyPos		= keyPos;
	btree->data->keyLen		= keyLen;
	btree->data->rootPos	= btree->position + sizeof(BTREEDATA);
	
	btree->root				= allocateNode(btree);
	btree->root->position	= btree->data->rootPos;
	NLEAF(btree->root)		= 1;
	
	writeBtree(btree);
	writeNode(btree, btree->root);
}

BTREE*	allocateBtree()
{
	BTREE* btree	= (BTREE*)malloc(sizeof(BTREE));
	
	btree->data		= (BTREEDATA*)malloc(sizeof(BTREEDATA));
	btree->position = 0;
	btree->file		= 0;
	btree->root		= 0;
	
	return btree;
}

void	freeBtree(BTREE* btree)
{
	freeNode(btree->root);
	free(btree->data);
	free(btree);
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							    FILE OPERATIONS								  */
/* ************************************************************************** */

void	readBtree(BTREE* btree)
{
	fseek(btree->file, btree->position, SEEK_SET);
	fread(btree->data,sizeof(BTREEDATA),1,btree->file);
	btree->root = allocateNode(btree);
	btree->root->position = btree->data->rootPos;
	readNode(btree,btree->root);
}

void	writeBtree(BTREE* btree)
{
	fseek(btree->file, btree->position, SEEK_SET);
	fwrite(btree->data,sizeof(BTREEDATA),1,btree->file);
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							     B-TREE SEARCH								  */
/* ************************************************************************** */

void	recursiveSearch(BTSEARCH* p)
{
	SHORT i = 1;
	
	while (i <= NRECNUM(p->node)
		&& memcmp(p->key, getKey(p->btree,p->node,i), BKEYLEN(p->btree) > 0))
		i++;
	
	if(i <= NRECNUM(p->node)
		&& memcmp(p->key, getKey(p->btree,p->node,i), BKEYLEN(p->btree) == 0))
	{
		p->position = i;
		p->result = SEARCH_FOUND;
		return;
	}
	
	if(NLEAF(p->node) > 0)
	{
		p->result = SEARCH_NOTFOUND;
		return;
	}
	else
	{
		p->node->position = getChild(p->node,i);
		readNode(p->btree, p->node);
		recursiveSearch(p);
	}
	
	p->result = SEARCH_NOTFOUND;
}

int		BtreeSearch(BTREE* btree, char* key, char* record)
{
	BTSEARCH* p = (BTSEARCH*)malloc(sizeof(BTSEARCH));
	
	p->btree = btree;
	p->key = key;
	p->node = allocateNode(btree);
	
	/* make a copy of the root node */
	NLEAF(p->node) = NLEAF(btree->root);
	NRECNUM(p->node) = NRECNUM(btree->root);
	memcpy(NRECORDS(p->node), NRECORDS(btree->root), BMAXREC(btree) * BRECLEN(btree));
	memcpy(NCHILDREN(p->node), NCHILDREN(btree->root), BDEGREE(btree)*2*sizeof(LONG));
	
	recursiveSearch(p);
	
	if(p->result == SEARCH_FOUND)
		memcpy(record, getRecord(btree, p->node, p->position), BRECLEN(btree));
	
	freeNode(p->node);
	
	return p->result;
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							   B-TREE INSERTION								  */
/* ************************************************************************** */

int		BtreeInsert(BTREE* btree, char* record)
{
	return 0;
}

/* ********************************** *** *********************************** */
