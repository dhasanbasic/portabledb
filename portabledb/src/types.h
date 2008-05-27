#ifndef TYPES_H_
#define TYPES_H_

#include <stdio.h>

typedef unsigned short int	SHORT;
typedef long int			LONG;

/******************************************************************************
 * 							    DATA STRUCTURES								  *
 ******************************************************************************/

typedef struct {
	SHORT	degree;		/* minimum number of child nodes */
	SHORT	keyPos;
	SHORT	keyLen;
	SHORT	recordLen;
	LONG	rootPos;
} BTREEDATA;

typedef struct {
	char	isLeaf;
	SHORT	recordNum;
	char*	records;
	LONG*	children;
} BTNODEDATA;

/******************************************************************************
 * 							 IN-MEMORY DATA STRUCTURES						  *
 ******************************************************************************/

typedef struct {
	BTNODEDATA*	data;
	LONG		position;
} BTNODE;

typedef struct {
	BTREEDATA* 	data;
	FILE*		file;
	LONG		position;
	BTNODE*		root;
} BTREE;

/******************************************************************************
 * 					   B-TREE OPERATIONAL DATA STRUCTURES					  *
 ******************************************************************************/

typedef struct {
	BTREE*	btree;
	BTNODE*	node;
	char*	key;
	char*	record;
} BTPARAM;

#define SEARCH_FOUND		1
#define SEARCH_NOTFOUND		0

/******************************************************************************
 * 							     HELPER MACROS								  *
 ******************************************************************************/

/**
 * B-tree macros
 */
#define BDEGREE(b)		b->data->degree
#define BMINREC(b)		(BDEGREE(b)-1)
#define BMAXREC(b)		(BDEGREE(b)*2-1)
#define BKEYPOS(b)		b->data->keyPos
#define BKEYLEN(b)		b->data->keyLen
#define BRECLEN(b)		b->data->recordLen
#define BROOT(b)		b->data->rootPos

/**
 * Node macros
 */
#define NRECNUM(n)		n->data->recordNum
#define NRECORDS(n)		n->data->records
#define NCHILDREN(n)	n->data->children
#define NLEAF(n)		n->data->isLeaf

#endif /*TYPES_H_*/
