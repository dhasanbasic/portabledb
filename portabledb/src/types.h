#ifndef TYPES_H_
#define TYPES_H_

#include <stdio.h>

typedef unsigned int 	UINT;
typedef unsigned long 	LONG;

/**
 * B-tree tag (data in a file)
 */
struct BTAG
{
	LONG	root;		/* position of the root node of the B-tree		*/
	UINT 	order;		/* order of the B-tree							*/
	UINT	rSize;		/* size of a record								*/
	UINT 	kPos;		/* position of a records' key in a record		*/
	UINT 	kSize;		/* size of a records' key						*/		
};

typedef struct BTAG BTAG;

/**
 * B-tree structure
 */
struct BTREE
{
	FILE* 	file;			/* pointer to the file that contains the B-tree	*/
	LONG	position;		/* position of the B-tree in the file			*/
	BTAG*	tag;
};

typedef struct BTREE BTREE;

/**
 * Node structure
 */
struct NODE
{
	BTREE*	btree;			/* pointer to the B-tree that contains the node	*/
	LONG	position;		/* position of the node in the file				*/
	UINT 	records;		/* number of records in the node				*/
	char*	data;			/* pointer to the data records of the node		*/
	LONG* 	subnodes;		/* array of subnodes (positions) of the node	*/
};

typedef struct NODE NODE;


/**
 * B-tree operation structure
 * 
 * INFO:	Needed as a parameter for a B-tree search,
 * 		 	insert and delete record operation
 */
struct BPARAM
{
	NODE*	node;			/* pointer to the last processed node			*/
	char*	record;			/* pointer processed record						*/
	char*	key;			/* pointer to the key of the processed record	*/
	UINT	position;		/* position of the processed record in the node	*/
};

typedef struct BPARAM BPARAM;

#endif /*TYPES_H_*/
