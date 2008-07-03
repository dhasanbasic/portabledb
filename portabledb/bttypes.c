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
		+ (btnodemeta->maxRecords + 1)*sizeof(LONG);

	btnodemeta->posCount	= btnodemeta->posLeaf + sizeof(SHORT);
	btnodemeta->nodeLength	= btnodemeta->posCount + sizeof(SHORT);

	btnodemeta->recordLength	= btmeta->recordLength;
	btnodemeta->keyPosition		= btmeta->keyPosition;
	btnodemeta->keyLength		= btmeta->keyLength;
}

char* GetRecord(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT index)
{
	return (char*)(node->data + (index-1)*nodemeta->recordLength);
}

char* GetKey(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT index)
{
	return (char*)(node->data + (index-1)*nodemeta->recordLength
			+ nodemeta->keyPosition);
}

LONG* GetChild(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT index)
{
	return (LONG*)(node->data + nodemeta->posChildren + (index-1)*sizeof(LONG));
}

SHORT GetLeaf(
		const BtNodeMeta* nodemeta,
		const BtNode* node)
{
	return *((SHORT*)(node->data + nodemeta->posLeaf));
}

void SetLeaf(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT value)
{
	*((SHORT*)(node->data + nodemeta->posLeaf)) = value;
}

SHORT GetCount(
		const BtNodeMeta* nodemeta,
		const BtNode* node)
{
	return *((SHORT*)(node->data + nodemeta->posCount));
}

void SetCount(
		const BtNodeMeta* nodemeta,
		const BtNode* node,
		const SHORT value)
{
	*((SHORT*)(node->data + nodemeta->posCount)) = value;
}
