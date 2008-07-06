/*
 * bthelper.c
 *
 *  Created on: Jul 6, 2008
 *      Author: dinko
 */
#include "btree.h"

#include <malloc.h>
#include <string.h>

void	BtreeSearch(BtSearchParam* p)
{
	SHORT i = 1;
	SHORT keyLength = p->node->tree->meta->keyLength;

	while (i <= GetCount(p->node)
		&& (memcmp(p->key, GetKey(p->node,i), keyLength) > 0)) i++;

	if (i <= GetCount(p->node)
		&& (memcmp(p->key, GetKey(p->node,i), keyLength) == 0))
	{
		p->result = SEARCH_FOUND;
		p->index = i;
		return;
	}

	if (GetLeaf(p->node) == LEAF)
	{
		p->result = SEARCH_NOTFOUND;
		return;
	}
	else
	{
		if(p->node->position == p->node->tree->meta->rootPosition)
			p->node = AllocateNode(p->node->tree,MODE_MEMORY);

		p->node->position = GetChild(p->node,i);
		ReadNode(p->node);
		BtreeSearch(p);
	}
}

void	BtreeInsert(BtTree* tree, const void* record)
{
	BtNode* root = (BtNode*)tree->root;
	BtInsertParam* p = (BtInsertParam*)malloc(sizeof(BtInsertParam));

	p->record	= (char*)record;
	p->key		= p->record + tree->meta->keyPosition;

	if(GetCount(root) == tree->maxRecords)
	{
		/* replace the root by a new empty root with one child */
		BtNode* newRoot = AllocateNode(tree,MODE_FREELIST);
		tree->root = newRoot;
		SetLeaf(newRoot,INTERNAL);
		SetCount(newRoot,0);
		SetChild(newRoot,1,root->position);

		/* split the old root */
		BtreeSplitChild(newRoot,1,root);
		tree->meta->rootPosition = newRoot->position;

		/* continue the insertion */
		p->node = newRoot;
		BtreeInsertNonfull(p);
	}
	else
	{
		p->node = root;
		BtreeInsertNonfull(p);
	}
}

void	BtreeSplitChild(
			BtNode* x,
			const SHORT i,
			BtNode* y)
{
	BtTree* tree		= x->tree;
	BtNode* z			= AllocateNode(tree,MODE_FREELIST);
	SHORT order			= tree->meta->order;
	SHORT recordLength	= tree->meta->recordLength;
	SHORT minRecords	= tree->minRecords;

	/* update the new child node (z) */
	SetLeaf(z, GetLeaf(y));
	SetCount(z, minRecords);

	memcpy(GetRecord(z,1), GetRecord(y,order+1), minRecords * recordLength);

	if (GetLeaf(y) == INTERNAL)
	{
		memcpy(GetChildPtr(z,1), GetChildPtr(y,order+1), order * sizeof(LONG));
	}

	WriteNode(z);

	/* update the split child node */
	SetCount(y, minRecords);
	WriteNode(y);

	/* update the parent node */
	memmove(GetChildPtr(x,i+2), GetChildPtr(x,i+1),
			(GetCount(x)-i+1)*sizeof(LONG));

	memmove(GetRecord(x,i+1), GetRecord(x,i),
			(GetCount(x)-i+1)*recordLength);

	SetChild(x,i+1,z->position);
	memcpy(GetRecord(x,i), GetRecord(y,order), recordLength);

	SetCount(x, GetCount(x)+1);

	WriteNode(x);
}

void	BtreeInsertNonfull(BtInsertParam* p)
{
	SHORT i = GetCount(p->node);
	SHORT keyLength = p->node->tree->meta->keyLength;
	SHORT recordLength = p->node->tree->meta->recordLength;
	BtNode* child;

	while (i>=1 && (memcmp(p->key, GetKey(p->node,i), keyLength) < 0)) i--;

	if(GetLeaf(p->node) == LEAF)
	{
		/* shift a few records (if needed to) */
		if((GetCount(p->node) - i) > 0)
		{
			memmove(GetRecord(p->node,i+1),
					GetRecord(p->node,i),
					GetCount(p->node)-i);
		}
		/* insert the record and update the node */
		memcpy(GetRecord(p->node,i+1), p->record, recordLength);
		SetCount(p->node,GetCount(p->node)+1);
		WriteNode(p->node);
	}
	else
	{
		/* traverse to the right child node of the i-th record */
		i++;

		child = AllocateNode(p->node->tree,MODE_MEMORY);
		child->position = GetChild(p->node,i);
		ReadNode(child);

		/* is it a full child node? */
		if(GetCount(child) == p->node->tree->maxRecords)
			BtreeSplitChild(p->node,i,child);

		if(memcmp(p->key, GetKey(p->node,i), keyLength) > 0)
			i++;

		/* free unnecessary allocated resources */
		if(p->node->position != p->node->tree->meta->rootPosition)
		{
			free(p->node->data);
			free(p->node);
		}

		/* recursion */
		p->node = child;
		BtreeInsertNonfull(p);
	}
}
