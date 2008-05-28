
#include "btreelayer.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* ************************************************************************** */
/* 							    FILE OPERATIONS								  */
/* ************************************************************************** */

void NodeRead(BNODE* node)
{
	fseek(node->file, node->position, SEEK_SET);
	fread(node->data,NLEN(node),1,node->file);
}

void NodeWrite(BNODE* node)
{
	if(node->position == 0)
	{
		fseek(node->file, 0, SEEK_END);
		node->position = ftell(node->file);
	}
	else
		fseek(node->file, node->position, SEEK_SET);
	
	fwrite(node->data,NLEN(node),1,node->file);
}

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							     BTNODE OPERATIONS							  */
/* ************************************************************************** */
/*
void	SplitNode(BTREE* btree, NODE* parent, NODE* child, SHORT index)
{
	
}
*/
/* ********************************** *** *********************************** */
