/*
 * btreetypes.h
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#ifndef BTREETYPES_H_
#define BTREETYPES_H_

#include <stdio.h>

/* ************************************************************************* */
/*                             Meta-data structures
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


void calculateNodeMeta(BtMeta* btmeta, BtNodeMeta* btnodemeta)
{
	btnodemeta->minRecords	= btmeta->order - 1;
	btnodemeta->maxRecords	= btmeta->order*2 - 1;
	btnodemeta->posChildren	= btnodemeta->maxRecords * btmeta->recordLength;

	btnodemeta->posLeaf		= btnodemeta->posChildren
		+ (btnodemeta->maxRecords + 1)*sizeof(long int);

	btnodemeta->posCount	= btnodemeta->posLeaf + sizeof(long int);
	btnodemeta->nodeLength	= btnodemeta->posCount + sizeof(long int);

	btnodemeta->recordLength	= btmeta->recordLength;
	btnodemeta->keyPosition		= btmeta->keyPosition;
	btnodemeta->keyLength		= btmeta->keyLength;
}

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree node structure
/* ************************************************************************* */

typedef struct {

	char*			data;
	BtNodeMeta*		meta;

} BtNode;

char* getRecord(const BtNode* node, const unsigned short int position)
{
	return (char*)(node->data + (position-1)*node->meta->recordLength);
}

char* getKey(const BtNode* node, const unsigned short int position)
{
	return (char*)(node->data + (position-1)*node->meta->recordLength
			+ node->meta->keyPosition);
}

long int* getChild(const BtNode* node, const unsigned short int position)
{
	return (long int*)(node->data + node->meta->posChildren
			+ (position-1)*sizeof(long int));
}

unsigned short int isLeaf(const BtNode* node)
{
	return *((unsigned short int*)(node->data + node->meta->posLeaf));
}

void setLeaf(const BtNode* node, const unsigned short int value)
{
	*((unsigned short int*)(node->data + node->meta->posLeaf)) = value;
}

/* ************************************************************************* */

/* ************************************************************************* */
/*                           B-tree data structure
/* ************************************************************************* */

typedef struct {

	char*			data;
	BtNodeMeta*		meta;

} BtNode;

/* ************************************************************************* */

#endif /* BTREETYPES_H_ */
