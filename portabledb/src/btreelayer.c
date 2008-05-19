
#include "btreelayer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* ************************************************************************** */
/* 							   MEMORY OPERATIONS							  */
/* ************************************************************************** */

void	createBtree(BTREE*		btree,
					FILE*		file,
					const SINT	order,
					const SINT	recordSize,
					const SINT	keyPosition,
					const SINT	keySize)
{
	btree->file 				= file;
	btree->position 			= 0;
	
	btree->tag->order			= order;
	btree->tag->recordSize		= recordSize;
	btree->tag->keyPosition		= keyPosition;
	btree->tag->keySize			= keySize;
	btree->tag->rootPosition	= 0;	
}

BTREE*	allocateBtree()
{
	BTREE* btree 				= (BTREE*)malloc(sizeof(BTREE));
	btree->tag					= (BTREETAG*)malloc(sizeof(BTREETAG));
	return btree;
}

void	freeBtree(BTREE* btree)
{
	free(btree->tag);
	free(btree);
}

NODE*	allocateNode(BTREE* btree)
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
	free(node->tag);
	free(node);
}

void	moveRecords(NODE* node, SINT srcPos, SINT destPos, SINT count)
{
	char* srcPtr;
	char* destPtr;
	SINT size;
	
	srcPtr 	= MRECORD(node,srcPos);
	destPtr	= MRECORD(node,destPos);
	size	= count * MRECSIZE(node->btree);
	
	memmove(destPtr, srcPtr, size);
}

void	moveSubnodes(NODE* node, SINT srcPos, SINT destPos, SINT count)
{
	LONG* srcPtr;
	LONG* destPtr;
	SINT size;
	
	srcPtr 	= &MSUBNODE(node,srcPos);
	destPtr	= &MSUBNODE(node,destPos);
	size	= count * sizeof(LONG);
	
	memmove(destPtr, srcPtr, size);
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
	
	memcpy(buff+0,				 &MRECORDS(node), numSize);
	memcpy(buff+numSize,		 MDATA(node), datSize);
	memcpy(buff+numSize+datSize, MSUBNODES(node), subSize);
	
	fseek(node->btree->file, node->position, SEEK_SET);
	fwrite(buff,buffSize,1,node->btree->file);
}

void	loadNode(NODE* node)
{
	unsigned int numSize 	= sizeof(SINT);
	unsigned int datSize 	= MORDER(node->btree) * MRECSIZE(node->btree);
	unsigned int subSize 	= (MORDER(node->btree)+1) * sizeof(LONG);
	unsigned int buffSize	= numSize + datSize + subSize; 	
	
	char* buff = (char*)malloc(buffSize);
	
	fseek(node->btree->file, node->position, SEEK_SET);
	fread(buff,buffSize,1,node->btree->file);
	
	memcpy(&MRECORDS(node),	buff+0, numSize);
	memcpy(MDATA(node),		buff+numSize, datSize);
	memcpy(MSUBNODES(node),	buff+numSize+datSize, subSize);
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							     B-TREE SEARCH								  */
/* ************************************************************************** */

int		recursiveSearch(BTREESEARCH* p)
{
	BTREE* btree = p->node->btree;
	SINT i;
	
	loadNode(p->node);
	
	if (MRECORDS(p->node) == 0) return SEARCH_EMPTY;
	
	/* should the search be continued in the left subnode? */
	if (memcmp(p->key, MKEY(p->node,0), MKEYSIZE(btree)) < 0)
	{
		p->position = 0;
		if(MSUBNODE(p->node,0) == 0) return SEARCH_FIRST;
		p->node->position = MSUBNODE(p->node,0);
		recursiveSearch(p);
	}
	
	/* should the search be continued in the right subnode? */
	if (memcmp(p->key, MKEY(p->node,(MRECORDS(p->node)-1)), MKEYSIZE(btree)) > 0)
	{
		p->position = MRECORDS(p->node);
		if(MSUBNODE(p->node,MRECORDS(p->node)) == 0) return SEARCH_LAST;
		p->node->position = MSUBNODE(p->node,MRECORDS(p->node));
		recursiveSearch(p);
	}
	
	/* the record is perhaps inside of the current node */
	for (i=0; i<MRECORDS(p->node); i++)
	{
		if (memcmp(p->key, MKEY(p->node,i), MKEYSIZE(btree)) == 0)
		{
			/* the record was found */
			p->record = (char*)malloc(MRECSIZE(btree));
			memcpy(p->record, MRECORD(p->node,i), MRECSIZE(btree));
			p->position = i;
			return SEARCH_FOUND;
		}
		
		if (i == MRECORDS(p->node)-1) continue;
		
		/* the record could be in the appropriate subnode */
		if (   memcmp(p->key, MKEY(p->node,i), MKEYSIZE(btree)) > 0
			&& memcmp(p->key, MKEY(p->node,i+1), MKEYSIZE(btree)) < 0)
		{
			p->position = 1;
			if(MSUBNODE(p->node,i) == 0) return SEARCH_INSIDE;
			p->node->position = MSUBNODE(p->node,i);
			recursiveSearch(p);
		}
	}
	
	return 0;
}

int		searchRecord(BTREE* btree, char* key)
{
	return 0;
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							   B-TREE INSERTION								  */
/* ************************************************************************** */

int		insertRecord(BTREE* btree, void* record)
{
	BTREESEARCH* p = (BTREESEARCH*)malloc(sizeof(BTREESEARCH));
	int result;
	
	p->key = (char*)record+MKEYPOS(btree);
	p->node = allocateNode(btree);
	p->node->position = MROOT(btree);
	p->record = NULL;
	
	result = recursiveSearch(p);
	
	switch (result)
	{
	/* the root node is "empty" */
	case SEARCH_EMPTY:
		
		memcpy(MRECORD(p->node,0), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
		break;
		
	/* insert the record before the first record */
	case SEARCH_FIRST:
		
		/* shift the old records */
		moveRecords(p->node, 0, 1, MRECORDS(p->node));
		moveSubnodes(p->node, 0, 1, MRECORDS(p->node)+1);
		/* insert the new record */
		memcpy(MDATA(p->node), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
		MSUBNODE(p->node,0) = 0;
		break;
		
	/* insert the record after the last record */
	case SEARCH_LAST:
		
		memcpy(MRECORD(p->node, MRECORDS(p->node)), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
		break;
		
	/* insert the record after "p->position"-th record */
	case SEARCH_INSIDE:
		
		/* shift the old record */
		moveRecords(p->node, p->position, p->position+1,
				MRECORDS(p->node) - p->position);
		
		moveSubnodes(p->node, p->position+1, p->position+2,
				MRECORDS(p->node) - p->position);

		/* insert the new record */
		memcpy(MRECORD(p->node,p->position), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
		MSUBNODE(p->node,p->position+1) = 0;
		break;
		
	case SEARCH_FOUND:
		printf("(WW) record already exists (%d)\n", p->position);
	default:
		freeNode(p->node);
		free(p);
		return -1;
	}
	
	/* SPLIT */
	
	printf("(**) record has been inserted (%d)\n", p->position);
	
	saveNode(p->node);
	freeNode(p->node);
	free(p);
	return 1;
}

/* ********************************** *** *********************************** */
