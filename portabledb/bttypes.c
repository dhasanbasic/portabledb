/*
 * bttypes.c
 *
 *  Created on: Jul 2, 2008
 *      Author: dinko
 */

#include "bttypes.h"

void CalculateNodeMeta(BtMeta* btmeta, BtNodeMeta* btnodemeta)
{
	btnodemeta->minRecords	= btmeta->order - 1;
	btnodemeta->maxRecords	= btmeta->order*2 - 1;
	btnodemeta->posChildren	= btnodemeta->maxRecords * btmeta->recordLength;

	btnodemeta->posLeaf		= btnodemeta->posChildren
		+ (btnodemeta->maxRecords + 1)*sizeof(long int);

	btnodemeta->posCount	= btnodemeta->posLeaf + sizeof(unsigned short int);
	btnodemeta->nodeLength	= btnodemeta->posCount + sizeof(unsigned short int);

	btnodemeta->recordLength	= btmeta->recordLength;
	btnodemeta->keyPosition		= btmeta->keyPosition;
	btnodemeta->keyLength		= btmeta->keyLength;
}

char* GetRecord(const BtNode* node, const unsigned short int position)
{
	return (char*)(node->data + (position-1)*node->meta->recordLength);
}

char* GetKey(const BtNode* node, const unsigned short int position)
{
	return (char*)(node->data + (position-1)*node->meta->recordLength
			+ node->meta->keyPosition);
}

long int* GetChild(const BtNode* node, const unsigned short int position)
{
	return (long int*)(node->data + node->meta->posChildren
			+ (position-1)*sizeof(long int));
}

unsigned short int GetLeaf(const BtNode* node)
{
	return *((unsigned short int*)(node->data + node->meta->posLeaf));
}

void SetLeaf(const BtNode* node, const unsigned short int value)
{
	*((unsigned short int*)(node->data + node->meta->posLeaf)) = value;
}

unsigned short int GetCount(const BtNode* node)
{
	return *((unsigned short int*)(node->data + node->meta->posCount));
}

void SetCount(const BtNode* node, const unsigned short int value)
{
	*((unsigned short int*)(node->data + node->meta->posCount)) = value;
}
