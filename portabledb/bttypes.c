/*
 * bttypes.c
 *
 *  Created on: Jul 2, 2008
 *      Author: dinko
 */

#include "bttypes.h"

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

unsigned short int getLeaf(const BtNode* node)
{
	return *((unsigned short int*)(node->data + node->meta->posLeaf));
}

void setLeaf(const BtNode* node, const unsigned short int value)
{
	*((unsigned short int*)(node->data + node->meta->posLeaf)) = value;
}

unsigned short int getCount(const BtNode* node)
{
	return *((unsigned short int*)(node->data + node->meta->posCount));
}

void setCount(const BtNode* node, const unsigned short int value)
{
	*((unsigned short int*)(node->data + node->meta->posCount)) = value;
}
