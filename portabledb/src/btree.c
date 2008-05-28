
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
	btree->root = NodeAllocate(btree);
	
	btree->root->position = btree->meta->rootPosition;
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

int	TreeSearch(BTREE* btree, char* key, char* record)
{
	BTPARAM* p = (BTPARAM*)malloc(sizeof(BTPARAM));
	int result;
	
	p->key = key;
	p->record = record;
	
	/* make a copy of the root node */
	p->node = NodeAllocate(btree);
	
	LEAF(p->node) = LEAF(btree->root);
	COUNT(p->node) = COUNT(btree->root);
	
	memcpy(RECORD_PTR(p->node,0), RECORD_PTR(btree->root,0),
		   RMAX(btree->root) * RLEN(btree->root));
		   
	memcpy(CHILD_PTR(p->node,0), CHILD_PTR(btree->root,0),
		   (RMAX(btree->root)+1) * sizeof(LONG));
	
	result = SearchRecursive(p);
	
	NodeFree(p->node);
	free(p);
	
	return result;
}


int SearchRecursive(BTPARAM* p)
{
	SHORT i = 0;
	
	while (i+1 <= COUNT(p->node)
		&& memcmp(p->key, KEY_PTR(p->node,i), KLEN(p->node)) > 0)
		i++;
	
	while (i+1 <= COUNT(p->node)
		&& memcmp(p->key, KEY_PTR(p->node,i), KLEN(p->node)) == 0)
	{
		memcpy(p->record, RECORD_PTR(p->node,i), RLEN(p->node));
		return SEARCH_FOUND;
	}
	
	if(LEAF(p->node) > 0)
		return SEARCH_NOTFOUND;
	else
	{
		p->node->position = CHILD(p->node,i);
		NodeRead(p->node);
		SearchRecursive(p);
	}
	
	return SEARCH_NOTFOUND;
}

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
