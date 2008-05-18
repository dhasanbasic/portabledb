
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
	
	/* should the search be continued in the left subnode? */
	if (memcmp(p->key, MKEY(p->node,0), MKEYSIZE(btree)) < 0)
	{
		p->position = 0;
		if(MSUBNODE(p->node,0) == 0) return -1;
		p->node->position = MSUBNODE(p->node,0);
		recursiveSearch(p);
	}
	
	/* should the search be continued in the right subnode? */
	if (memcmp(p->key, MKEY(p->node,MRECORDS(p->node)-1), MKEYSIZE(btree)) > 0)
	{
		p->position = MRECORDS(p->node);
		if(MSUBNODE(p->node,MRECORDS(p->node)) == 0) return 1;
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
			return 0;
		}
		
		if (i == MRECORDS(p->node)-1) continue;
		
		/* the record could be in the appropriate subnode */
		if (   memcmp(p->key, MKEY(p->node,i), MKEYSIZE(btree)) > 0
			&& memcmp(p->key, MKEY(p->node,i+1), MKEYSIZE(btree)) < 0)
		{
			p->position = i;
			if(MSUBNODE(p->node,i) == 0) return 0;
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
	p->position = MROOT(btree);
	p->record = NULL;
	
	result = recursiveSearch(p);
	
	if (result < 0)
	{
		/* make place for the new record */
		memmove(MRECORD(p->node,1), MRECORD(p->node,0),
				MRECORDS(p->node) * MRECSIZE(btree));
		
		memmove(&MSUBNODES(p->node)[1], &MSUBNODES(p->node)[0],
				(MRECORDS(p->node)+1) * sizeof(LONG));
		
		/* insert the record */
		memcpy(MRECORD(p->node,0), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
		MSUBNODE(p->node,0) = 0;
	}
	else if (result > 0)
	{
		/* insert the record */
		memcpy(MRECORD(p->node,MRECORDS(p->node)), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
	}
	else
	{
		if (p->record != NULL)
		{
			freeNode(p->node);
			free(p);
			return -1;
		}
		
		/* make place for the new record */
		memmove(MRECORD(p->node,p->position+1), MRECORD(p->node,p->position),
				(MRECORDS(p->node) - p->position) * MRECSIZE(btree));
		
		memmove(&MSUBNODES(p->node)[p->position+1],
				&MSUBNODES(p->node)[p->position],
				(MRECORDS(p->node) - p->position + 1) * sizeof(LONG));
		
		/* insert the record */
		memcpy(MRECORD(p->node,p->position), record, MRECSIZE(btree));
		MRECORDS(p->node)++;
		MSUBNODE(p->node,p->position) = 0;	
	}	
	
	/* SPLIT */
	
	freeNode(p->node);
	free(p);
	return 1;
}

/* ********************************** *** *********************************** */
