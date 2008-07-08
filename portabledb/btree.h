/*
 * btree.h
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#ifndef BTREE_H_
#define BTREE_H_

#include "btree/bttypes.h"
#include <stdio.h>

/* ************************************************************************* */
/*                              B-tree functions                             */
/* ************************************************************************* */

BtTree*	CreateTree(
			FILE*		file,
			const SHORT	order,
			const SHORT	recordLength,
			const SHORT	keyPosition,
			const SHORT	keyLength,
			const SHORT	freelistSize);

void	WriteTree(BtTree* tree);

BtTree*	ReadTree(FILE* file, const LONG position);

void	FreeTree(BtTree* tree);

#define INSERTION_FAILED	-1
#define INSERTION_SUCCEEDED	 1

int		InsertRecord(BtTree* tree, const void* record);

int		SearchRecord(BtTree* tree, const void* key, void* record);

int		DeleteRecord(BtTree* tree, const void* key);

/* ************************************************************************* */

/* ************************************************************************* */
/*                              Node functions                             */
/* ************************************************************************* */

#define MODE_FREELIST	0
#define MODE_MEMORY		1

BtNode*	AllocateNode(BtTree* tree, const int mode);

void	WriteNode(BtNode* node);

void	ReadNode(BtNode* node);

/* ************************************************************************* */

/* ************************************************************************* */
/*                              Helper functions                             */
/* ************************************************************************* */

void	BtreeSearch(
			BtNode* node,
			const char* key,
			BtSearchResult* p);

void	BtreeInsert(BtTree* tree, const void* record);

void	BtreeSplitChild(
			BtNode* parent,
			const SHORT index,
			BtNode* fullchild);

void	BtreeInsertNonfull(BtInsertParam* p);

void	BtreeMergeNodes(
			BtNode* x,
			BtNode* y,
			const SHORT i,
			BtNode* z);

#define TYPE_LEFT	-1
#define TYPE_RIGHT	 1

void	BtreeReduceSibling(
			BtNode* parent,
			BtNode* child,
			BtNode* sibling,
			const SHORT i,
			const int type);

int		BtreeDelete(BtNode* node, const void* key);

/* ************************************************************************* */

#endif /* BTREE_H_ */
