
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

int		recursiveSearch(BTPARAM* p)
{
	SHORT i = 1;
	
	while (i <= NRECNUM(p->node)
		&& memcmp(p->key, getKey(p->btree,p->node,i), BKEYLEN(p->btree) > 0))
		i++;
	
	if(i <= NRECNUM(p->node)
		&& memcmp(p->key, getKey(p->btree,p->node,i), BKEYLEN(p->btree) == 0))
	{
		memcpy(p->record, getRecord(p->btree,p->node, i), BRECLEN(p->btree));
		return SEARCH_FOUND;
	}
	
	if(NLEAF(p->node) > 0)
		return SEARCH_NOTFOUND;
	else
	{
		p->node->position = getChild(p->node,i);
		readNode(p->btree, p->node);
		recursiveSearch(p);
	}
	
	return SEARCH_NOTFOUND;
}

int		BtreeSearch(BTREE* btree, char* key, char* record)
{
	BTPARAM* p = (BTPARAM*)malloc(sizeof(BTPARAM));
	int result;
	
	p->btree = btree;
	p->key = key;
	p->record = record;
	
	/* make a copy of the root node */
	p->node = allocateNode(btree);
	
	NLEAF(p->node) = NLEAF(btree->root);
	NRECNUM(p->node) = NRECNUM(btree->root);
	memcpy(NRECORDS(p->node), NRECORDS(btree->root), BMAXREC(btree) * BRECLEN(btree));
	memcpy(NCHILDREN(p->node), NCHILDREN(btree->root), BDEGREE(btree)*2*sizeof(LONG));
	
	result = recursiveSearch(p);
	
	freeNode(p->node);
	free(p);
	
	return result;
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
