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

/* ************************************************************************* */
/*                             Meta-data structures                          */
/* ************************************************************************* */

typedef struct {

	unsigned short int	order;
	unsigned short int	recordLength;
	unsigned short int	keyPosition;
	unsigned short int	keyLength;
	long int			rootPosition;
	long int			freelistPosition;
	unsigned short int	freelistSize;
	unsigned short int	freeNodes;

} BtMeta;

typedef struct {

	unsigned short int	nodeLength;
	unsigned short int	minRecords;
	unsigned short int	maxRecords;
	unsigned short int	posChildren;
	unsigned short int	posLeaf;
	unsigned short int	posCount;
	unsigned short int	recordLength;
	unsigned short int	keyLength;
	unsigned short int	keyPosition;

} BtNodeMeta;


void CalculateNodeMeta(BtMeta* btmeta, BtNodeMeta* btnodemeta);

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree node structure                           */
/* ************************************************************************* */

typedef struct {

	char*			data;
	BtNodeMeta*		meta;

} BtNode;

char* GetRecord(const BtNode* node, const unsigned short int position);

char* GetKey(const BtNode* node, const unsigned short int position);

long int* GetChild(const BtNode* node, const unsigned short int position);

unsigned short int GetLeaf(const BtNode* node);
void SetLeaf(const BtNode* node, const unsigned short int value);

unsigned short int GetCount(const BtNode* node);
void SetCount(const BtNode* node, const unsigned short int value);

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree data structure                           */
/* ************************************************************************* */

typedef struct {

	BtMeta*			meta;
	BtNodeMeta*		nodemeta;
	BtNode*			root;
	long int*		freelist;
	FILE*			file;
	long int		position;

} BtTree;

/* ************************************************************************* */

#endif /* BTREETYPES_H_ */
