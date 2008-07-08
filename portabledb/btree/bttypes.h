/*
 * btreetypes.h
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#ifndef BTREETYPES_H_
#define BTREETYPES_H_

#include <stdio.h>

#define INTERNAL	0
#define LEAF		1

typedef unsigned short int	SHORT;
typedef unsigned long int	LONG;

/* ************************************************************************* */
/*                             Meta-data structures                          */
/* ************************************************************************* */

typedef struct {

	SHORT	order;
	SHORT	recordLength;
	SHORT	keyPosition;
	SHORT	keyLength;
	LONG	rootPosition;
	LONG	freelistPosition;
	SHORT	freelistSize;
	SHORT	freeNodes;

} BtMeta;

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree data structure                           */
/* ************************************************************************* */

typedef struct {

	BtMeta*		meta;
	void*		root;
	FILE*		file;
	LONG*		freelist;
	LONG		position;

	/* node meta-data */
	SHORT		nodeLength;
	SHORT		minRecords;
	SHORT		maxRecords;
	SHORT		posChildren;
	SHORT		posLeaf;
	SHORT		posCount;

} BtTree;

void CalculateNodeMeta(BtTree* tree);

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree node structure                           */
/* ************************************************************************* */

typedef struct {

	char*	data;
	LONG	position;
	BtTree* tree;

} BtNode;

char* GetRecord(
		const BtNode* node,
		const SHORT index);

char* GetKey(
		const BtNode* node,
		const SHORT index);

char* GetChildPtr(
		const BtNode* node,
		const SHORT index);

LONG GetChild(
		const BtNode* node,
		const SHORT index);

void SetChild(
		const BtNode* node,
		const SHORT index,
		const LONG value);

SHORT GetLeaf(const BtNode* node);

void SetLeaf(
		const BtNode* node,
		const SHORT value);

SHORT GetCount(const BtNode* node);

void SetCount(
		const BtNode* node,
		const SHORT value);

/* ************************************************************************* */

/* ************************************************************************* */
/*                     B-tree operations - structures                        */
/* ************************************************************************* */

typedef struct {
	SHORT	index;
	LONG	position;
	int		result;
} BtSearchResult;

#define SEARCH_FOUND	 1
#define SEARCH_NOTFOUND	-1

typedef struct {
	BtNode*	node;
	char*	key;
	char*	record;
} BtInsertParam;

typedef struct {
	BtNode*	node;
	char*	key;
	int		result;
} BtDeleteParam;

#define DELETION_SUCCEEDED	 1
#define DELETION_FAILED		-1

/* ************************************************************************* */

#endif /* BTREETYPES_H_ */
