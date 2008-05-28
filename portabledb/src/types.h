#ifndef TYPES_H_
#define TYPES_H_

#include <stdio.h>

typedef unsigned short int	SHORT;
typedef long int			LONG;

/******************************************************************************
 *				  		DATA STRUCTURES, HELPER MACROS						  *
 ******************************************************************************/

typedef struct {
	SHORT	degree;			/* minimum number of child nodes */
	SHORT	minRecords;
	SHORT	maxRecords;
	SHORT	keyPosition;
	SHORT	keyLength;
	SHORT	recordLength;
	SHORT	nodeLength;
	LONG	rootPosition;
} BTREEMETA;

typedef struct {
	char*		leaf;
	char*		recordCount;
	char*		records;
	char*		children;
	BTREEMETA*	meta;
	char*		data;
	FILE*		file;
	LONG		position;
} BNODE;

#define RLEN(node)			node->meta->recordLength
#define RMIN(node)			node->meta->minRecords
#define RMAX(node)			node->meta->maxRecords
#define KPOS(node)			node->meta->keyPosition
#define KLEN(node)			node->meta->keyLength
#define NLEN(node)			node->meta->nodeLength

#define LEAF(node)			*((SHORT*)node->leaf)
#define COUNT(node)			*((SHORT*)node->recordCount)
#define CHILD(node,i)		*((LONG*)(node->children + i*sizeof(LONG)))

#define RECORD_PTR(node,i)	node->records + i*RLEN(node)
#define KEY_PTR(node,i)		RECORD_PTR(node,i) + KPOS(node)
#define CHILD_PTR(node,i)	node->children + i*sizeof(LONG)

typedef struct {
	BTREEMETA*	meta;
	FILE*		file;
	LONG		position;
	BNODE*		root;
} BTREE;

/******************************************************************************
 * 					   		  ALLOCATION, DEALLOCATION					  	  *
 ******************************************************************************/

void	NodeInit(BNODE* node);

BNODE*	NodeAllocate(BTREE* btree);

void	NodeFree(BNODE* node);

BTREE*	TreeAllocate();

BTREE*	TreeCreate(FILE* file, SHORT degree, SHORT recordLength,
		           SHORT keyPosition, SHORT keyLength);

void	TreeFree(BTREE* btree);

/******************************************************************************
 * 					   B-TREE OPERATION PARAMETER STRUCTURE				  	  *
 ******************************************************************************/

typedef struct {
	BNODE*	node;
	char*	key;
	char*	record;
} BTPARAM;

#define SEARCH_FOUND		1
#define SEARCH_NOTFOUND		0

#endif /*TYPES_H_*/
