#ifndef BTREELAYER_H_
#define BTREELAYER_H_

#include "types.h"

#include <stdio.h>

/* ************************************************************************** */
/* 							   BTREE OPERATIONS							  */
/* ************************************************************************** */

BTREE* TreeRead(FILE* file, LONG position);
void TreeWrite(BTREE* btree);

//int		recursiveSearch(BTPARAM* p);
//int		BtreeSearch(BTREE* btree, char* key, char* record);

//int		BtreeInsert(BTREE* btree, char* record);

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							    BTNODE OPERATIONS							  */
/* ************************************************************************** */

void NodeRead(BNODE* node);
void NodeWrite(BNODE* node);

/* ********************************** *** *********************************** */

#endif /*BTREELAYER_H_*/
