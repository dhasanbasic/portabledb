#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned short	SHORT;
typedef long int		LONG;

/*
 * B-tree descriptor structure
 */
typedef struct
{
	SHORT	minChildren;
	SHORT	recordSize;
	SHORT	keySize;
	SHORT	keyOffset;
	LONG	rootPtr;
} BTREEDESC;


/*
 * Node descriptor structure
 */
typedef struct
{
	BTREEDESC*	btreedesc;
	SHORT		minRecords;
	SHORT		maxRecords;
	SHORT		nodeSize;
	SHORT		recordsOffset;
	SHORT		childrenOffset;
	SHORT		countOffset;
	SHORT		leafOffset;
} NODEDESC;


/*
 * Node structure
 */
typedef struct
{
	NODEDESC*	desc;
	char*		data;
} NODE;

/*
 * B-tree structure
 */
typedef struct
{
	NODEDESC*	desc;
	NODE*		root;
	LONG		position;
} BTREE;

/*
 * Helper macros
 */
#define RECSIZE(n)		n->desc->btreedesc->recordSize
#define KEYSIZE(n)		n->desc->btreedesc->keySize
#define KEYPOS(n)		n->desc->btreedesc->keyOffset
#define ROOTPTR(n)		n->desc->btreedesc->rootPtr

#define MINREC(n)		n->desc->minRecords
#define MAXREC(n)		n->desc->maxRecords
#define NODESIZE(n)		n->desc->nodeSize
#define RECORDS(n)		n->desc->recordsOffset
#define CHILDREN(n)		n->desc->childrenOffset
#define COUNT(n)		n->desc->countOffset
#define LEAF(n)			n->desc->leafOffset

#endif /*TYPES_H_*/
