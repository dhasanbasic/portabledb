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

BtNode*	AllocateNode(BtNodeMeta* nodemeta);

void	WriteNode(FILE* file, const BtNodeMeta* nodemeta, BtNode* node);

void	ReadNode(FILE* file, const BtNodeMeta* nodemeta, BtNode* node);

/* ************************************************************************* */

#endif /* BTREE_H_ */
