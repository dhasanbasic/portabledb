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

/* ************************************************************************* */

/* ************************************************************************* */
/*                              Node functions                             */
/* ************************************************************************* */

BtNode*	AllocateNode(BtTree* tree);

void	WriteNode(BtNode* node);

void	ReadNode(BtNode* node);

void	SplitChild(
			BtNode* parent,
			const SHORT index,
			BtNode* fullchild);

/* ************************************************************************* */

#endif /* BTREE_H_ */
