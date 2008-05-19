#ifndef BTREELAYER_H_
#define BTREELAYER_H_

#include "types.h"

/* ************************************************************************** */
/* 							   MEMORY OPERATIONS							  */
/* ************************************************************************** */

void	createBtree(BTREE* btree, FILE*	file, const SINT	order,
					const SINT	recordSize, const SINT	keyPosition,
					const SINT	keySize);
BTREE*	allocateBtree();
void	freeBtree(BTREE* btree);

NODE*	allocateNode(BTREE* btree);
void	freeNode(NODE* node);

/* ********************************** *** *********************************** */

/* ************************************************************************** */
/* 							    FILE OPERATIONS								  */
/* ************************************************************************** */

void	loadBtree(BTREE* btree);
void	saveBtree(BTREE* btree);

void	loadNode(NODE* node);
void	saveNode(NODE* node);

/* ********************************** *** *********************************** */

int		insertRecord(BTREE* btree, void* record);


#endif /*BTREELAYER_H_*/
