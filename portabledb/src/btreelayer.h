#ifndef BTREELAYER_H_
#define BTREELAYER_H_

#include "types.h"

#include <stdio.h>

/* ************************************************************************** */
/* 							   BTREE OPERATIONS							  */
/* ************************************************************************** */

void	createBtree(BTREE*		btree,
					FILE*		file,
					const SHORT	degree,
					const SHORT	recordLen,
					const SHORT	keyPos,
					const SHORT	keyLen);

BTREE*	allocateBtree();
void	freeBtree(BTREE* btree);

void	readBtree(BTREE* btree);
void	writeBtree(BTREE* btree);

void	recursiveSearch(BTSEARCH* p);
int		BtreeSearch(BTREE* btree, char* key, char* record);

int		BtreeInsert(BTREE* btree, char* record);

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							    BTNODE OPERATIONS							  */
/* ************************************************************************** */

BTNODE*	allocateNode(BTREE* btree);
void	freeNode(BTNODE* node);

char*	getRecord(BTREE* btree, BTNODE* node, SHORT position);
char*	getKey(BTREE* btree, BTNODE* node, SHORT position);
LONG	getChild(BTNODE* node, SHORT position);

void	readNode(BTREE* btree, BTNODE* node);
void	writeNode(BTREE* btree, BTNODE* node);

/* ********************************** *** *********************************** */

#endif /*BTREELAYER_H_*/
