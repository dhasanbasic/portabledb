
#include "btreelayer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* ************************************************************************** */
/* 							   MEMORY OPERATIONS							  */
/* ************************************************************************** */

BTNODE*	allocateNode(BTREE* btree)
{
	BTNODE* node			= (BTNODE*)malloc(sizeof(BTNODE));
	
	node->position			= 0;
	node->data				= (BTNODEDATA*)malloc(sizeof(BTNODEDATA));
	
	node->data->isLeaf		= 0;
	node->data->recordNum	= 0;
	node->data->records		= (char*)malloc(BMAXREC(btree) * BRECLEN(btree));
	node->data->children	= (LONG*)calloc(BDEGREE(btree)*2,sizeof(LONG));
	
	return node;
}

void	freeNode(BTNODE* node)
{
	cfree(node->data->children);
	free(node->data->records);
	free(node->data);
	free(node);
}

char*	getRecord(BTREE* btree, BTNODE* node, SHORT position)
{
	return NRECORDS(node) + (position-1) * BRECLEN(btree);
}

char*	getKey(BTREE* btree, BTNODE* node, SHORT position)
{
	return NRECORDS(node) + (position-1) * BRECLEN(btree) + BKEYPOS(btree);
}

LONG	getChild(BTNODE* node, SHORT position)
{
	return *(NCHILDREN(node) + (position-1)*sizeof(LONG));
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							    FILE OPERATIONS								  */
/* ************************************************************************** */

void	readNode(BTREE* btree, BTNODE* node)
{
	SHORT metadataLen	= sizeof(char) + sizeof(SHORT);
	SHORT recordsLen	= BMAXREC(btree) * BRECLEN(btree);
	SHORT childrenLen	= BDEGREE(btree)* 2 * sizeof(LONG);
	SHORT bufferLen		= metadataLen + recordsLen + childrenLen; 
	
	char* buffer = (char*)malloc(bufferLen);
	
	fseek(btree->file, node->position, SEEK_SET);
	fread(buffer,bufferLen,1,btree->file);

	memcpy(&NLEAF(node),	buffer,							sizeof(char));
	memcpy(&NRECNUM(node),	buffer+sizeof(char),			sizeof(SHORT));	
	memcpy(NRECORDS(node),	buffer+metadataLen,				recordsLen);
	memcpy(NCHILDREN(node),	buffer+metadataLen+recordsLen,	childrenLen);
}

void	writeNode(BTREE* btree, BTNODE* node)
{
	SHORT metadataLen	= sizeof(char) + sizeof(SHORT);
	SHORT recordsLen	= BMAXREC(btree) * BRECLEN(btree);
	SHORT childrenLen	= BDEGREE(btree)* 2 * sizeof(LONG);
	SHORT bufferLen		= metadataLen + recordsLen + childrenLen; 
	
	char* buffer = (char*)malloc(bufferLen);
	
	memcpy(buffer,							&NLEAF(node),	 sizeof(char));
	memcpy(buffer+sizeof(char),				&NRECNUM(node),	 sizeof(SHORT));	
	memcpy(buffer+metadataLen,				NRECORDS(node),	 recordsLen);
	memcpy(buffer+metadataLen+recordsLen,	NCHILDREN(node), childrenLen);
	
	if(node->position == 0)
	{
		fseek(btree->file, 0, SEEK_END);
		node->position = ftell(btree->file);
	}
	else
		fseek(btree->file, node->position, SEEK_SET);
	
	fwrite(buffer,bufferLen,1,btree->file);
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							     BTNODE OPERATIONS							  */
/* ************************************************************************** */


/* ********************************** *** *********************************** */
