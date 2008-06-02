#ifndef BTREELAYER_H_
#define BTREELAYER_H_

#include <stdio.h>

#include "types.h"

/*
 * Allocation and freeing of memory
 */

	void	freeBtree(BTREE* btree);

	NODE*	allocNode(NODEDESC* desc);
	void	freeNode(NODE* node);

/*
 * File operations
 */

	BTREE*	readBtree(FILE* file, LONG position);
	void	writeBtree(FILE* file, BTREE* btree);
	
	void	readNode(FILE* file, LONG position, NODE* node);
	LONG	writeNode(FILE* file, LONG position, NODE* node);

/*
 * B-tree operations
 */

	BTREE*	createBtree(FILE* file, BTREEDESC* desc);

/*
 * Node helper functions
 */

	char* 	getRecordPtr(NODE* node, SHORT n);
	char*	getKeyPtr(NODE* node, SHORT n);
	LONG* 	getChildPtr(NODE* node, SHORT n);
	SHORT	getCount(NODE* node);
	void	setCount(NODE* node, SHORT value);
	SHORT	getLeaf(NODE* node);
	void	setLeaf(NODE* node, SHORT value);

/*
 * Node operations
 */

#endif /*BTREELAYER_H_*/
