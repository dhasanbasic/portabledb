/*
 * btree.h
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#ifndef BTREE_H_
#define BTREE_H_

#include "bttypes.h"
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

void	ReadTree(BtTree* tree);

void	InsertRecord(BtTree* tree, const void* record);

int		SearchRecord(BtTree* tree, void* key, void* record);

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

void	BtreeSearch(BtSearchParam* p);

void	BtreeInsert(BtTree* tree, const void* record);

void	BtreeSplitChild(
			BtNode* parent,
			const SHORT index,
			BtNode* fullchild);

void	BtreeInsertNonfull(BtInsertParam* p);

/* ************************************************************************* */

#endif /* BTREE_H_ */
