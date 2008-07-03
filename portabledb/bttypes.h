/*
 * btreetypes.h
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#ifndef BTREETYPES_H_
#define BTREETYPES_H_

#include <stdio.h>

#define CHILD	0
#define LEAF	1

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

typedef struct {

	SHORT	nodeLength;
	SHORT	minRecords;
	SHORT	maxRecords;
	SHORT	posChildren;
	SHORT	posLeaf;
	SHORT	posCount;
	SHORT	recordLength;
	SHORT	keyLength;
	SHORT	keyPosition;
	SHORT	freelistSize;
	SHORT	freeNodes;
	LONG*	freelist;

} BtNodeMeta;


void CalculateNodeMeta(BtMeta* btmeta, BtNodeMeta* btnodemeta);

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree node structure                           */
/* ************************************************************************* */

typedef struct {

	char*	data;
	LONG	position;

} BtNode;

char* GetRecord(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT index);

char* GetKey(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT index);

LONG* GetChild(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT index);

SHORT GetLeaf(
		const BtNodeMeta* nodemeta,
		const BtNode* node);

void SetLeaf(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT value);

SHORT GetCount(
		const BtNodeMeta* nodemeta,
		const BtNode* node);

void SetCount(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT value);

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree data structure                           */
/* ************************************************************************* */

typedef struct {

	BtMeta*		meta;
	BtNodeMeta*	nodemeta;
	BtNode*		root;
	FILE*		file;
	LONG		position;

} BtTree;

/* ************************************************************************* */

#endif /* BTREETYPES_H_ */
