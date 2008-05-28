
#include "btreelayer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* ************************************************************************** */
/* 							    FILE OPERATIONS								  */
/* ************************************************************************** */

BTREE* TreeRead(FILE* file, LONG position)
{
	BTREE* btree	= (BTREE*)malloc(sizeof(BTREE));
	
	btree->meta		= (BTREEMETA*)malloc(sizeof(BTREEMETA));
	btree->file		= file;
	btree->position	= position;
	
	fseek(file, position, SEEK_SET);
	fread(btree->meta,sizeof(BTREEMETA),1,file);
	btree->root->position = btree->meta->rootPosition;
	
	btree->root = NodeAllocate(btree);
	NodeRead(btree->root);
	
	return btree;
}

void TreeWrite(BTREE* btree)
{
	fseek(btree->file, btree->position, SEEK_SET);
	fwrite(btree->meta,sizeof(BTREEMETA),1,btree->file);
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							     B-TREE SEARCH								  */
/* ************************************************************************** */
/*
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
*/
/*
int		BtreeSearch(BTREE* btree, char* key, char* record)
{
	BTPARAM* p = (BTPARAM*)malloc(sizeof(BTPARAM));
	int result;
	
	p->btree = btree;
	p->key = key;
	p->record = record;
	
	/* make a copy of the root node */
/*	p->node = allocateNode(btree);
	
	NLEAF(p->node) = NLEAF(btree->root);
	NRECNUM(p->node) = NRECNUM(btree->root);
	memcpy(NRECORDS(p->node), NRECORDS(btree->root), BMAXREC(btree) * BRECLEN(btree));
	memcpy(NCHILDREN(p->node), NCHILDREN(btree->root), BDEGREE(btree)*2*sizeof(LONG));
	
	result = recursiveSearch(p);
	
	freeNode(p->node);
	free(p);
	
	return result;
}
*/
/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							   B-TREE INSERTION								  */
/* ************************************************************************** */
/*
int		BtreeInsert(BTREE* btree, char* record)
{
	return 0;
}
*/
/* ********************************** *** *********************************** */
