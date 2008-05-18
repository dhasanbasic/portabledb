#ifndef TYPES_H_
#define TYPES_H_

#include <stdio.h>

typedef unsigned short int	SINT;
typedef long int			LONG;

/**
 * B-tree tag (B-tree data in a file)
 * 
 *	rootPosition	position of the root node
 *	order			order of the B-tree
 *	keyPosition		position of the record-key
 *	keySize			length of the key
 *	recordSize		length of the record
 */
typedef struct
{
	LONG	rootPosition;
	SINT	recordSize;
	SINT	order;
	SINT	keyPosition;
	SINT	keySize;
}
BTREETAG;


/**
 * B-tree structure
 * 
 *	file			pointer to the file that contains the B-tree
 *	position		position of the B-tree in the file
 * 	tag				the B-tree data
 */
typedef struct
{
	FILE*		file;
	LONG		position;
	BTREETAG*	tag;
}
BTREE;

/**
 * Node tag (data of a node in a file)
 * 
 *	records			number of records in the node
 *	data			pointer to the records in the node
 *	subnodes		array of subnodes of the node
 */
typedef struct
{
	SINT		records;
	char*		data;
	LONG*		subnodes;
}
NODETAG;

/**
 * Node structure
 * 
 *	btree			B-tree containing the node
 *	position		position of the node in the file
 *  tag				the data of the node
 */
typedef struct
{
	BTREE*		btree;
	LONG		position;
	NODETAG*	tag;
}
NODE;

/**
 * B-tree search structure
 * 
 * INFO:	Needed as a parameter for a B-tree search,
 * 		 	insert and delete record operation
 * 
 * 	node			pointer to the last processed node
 *	record			pointer to a record (searched, inserted, etc)
 *	key				pointer to the searched key
 *	position		position of the last processed record in a node
 */
typedef struct
{
	NODE*	node;
	char*	record;
	char*	key;
	SINT	position;
}
BTREESEARCH;

/******************************************************************************
 * 							HELPER MACROS									  *
 ******************************************************************************/

/**
 * B-tree macros
 */
#define	KEY_POSITION(btree)	btree->tag->keyPosition
#define KEY_SIZE(btree)		btree->tag->keySize
#define RECORD_SIZE(btree)	btree->tag->recordSize
#define ORDER(btree)		btree->tag->order
#define ROOT_NODE(btree)	btree->tag->root

/**
 * Node macros
 */
#define	DATA_AT(n,pos)		n->tag->data+pos
#define RECORD_AT(n,i)		n->tag->data+(i*RECORD_SIZE(n->btree))
#define KEY_AT(n,i)			RECORD_AT(n,i)+KEY_POSITION(n->btree)
#define SUBNODE(n,i)		n->tag->subnodes[i]


#endif /*TYPES_H_*/
