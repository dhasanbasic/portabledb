/*
 * bttypes.c
 *
 *  Created on: Jul 2, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */

#include "bttypes.h"

void CalculateNodeMeta(BtTree* tree)
{
	tree->minRecords	= tree->meta->order - 1;
	tree->maxRecords	= tree->meta->order*2 - 1;
	tree->posChildren	= tree->maxRecords * tree->meta->recordLength;

	tree->posLeaf		= tree->posChildren + (tree->maxRecords+1)*sizeof(LONG);

	tree->posCount		= tree->posLeaf + sizeof(SHORT);
	tree->nodeLength	= tree->posCount + sizeof(SHORT);
}

char* GetRecord(
		const BtNode* node,
		const SHORT index)
{
	return (char*)(node->data + (index-1)*node->tree->meta->recordLength);
}

char* GetKey(
		const BtNode* node,
		const SHORT index)
{
	return (char*)(node->data + (index-1)*node->tree->meta->recordLength
			+ node->tree->meta->keyPosition);
}

char* GetChildPtr(
		const BtNode* node,
		const SHORT index)
{
	return (char*)(node->data + node->tree->posChildren
			+ (index-1)*sizeof(LONG));
}

LONG GetChild(
		const BtNode* node,
		const SHORT index)
{
	return *((LONG*)(node->data + node->tree->posChildren
			+ (index-1)*sizeof(LONG)));
}

void SetChild(
		const BtNode* node,
		const SHORT index,
		const LONG value)
{
	*((LONG*)(node->data + node->tree->posChildren
			+ (index-1)*sizeof(LONG))) = value;
}

SHORT GetLeaf(const BtNode* node)
{
	return *((SHORT*)(node->data + node->tree->posLeaf));
}

void SetLeaf(
		const BtNode* node,
		const SHORT value)
{
	*((SHORT*)(node->data + node->tree->posLeaf)) = value;
}

SHORT GetCount(const BtNode* node)
{
	return *((SHORT*)(node->data + node->tree->posCount));
}

void SetCount(
		const BtNode* node,
		const SHORT value)
{
	*((SHORT*)(node->data + node->tree->posCount)) = value;
}
