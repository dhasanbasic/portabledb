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
			FILE*						file,
			const unsigned short int	order,
			const unsigned short int	recordLength,
			const unsigned short int	keyPosition,
			const unsigned short int	keyLength,
			const unsigned short int	freelistSize);

void	WriteTree(const BtTree*	tree);

void	ReadTree(BtTree* tree);

/* ************************************************************************* */

/* ************************************************************************* */
/*                              Node functions                             */
/* ************************************************************************* */

BtNode*	AllocateNode(BtNodeMeta* nodemeta);

void	WriteNode(FILE* file, BtNode* node, const long int position);

void	ReadNode(FILE* file, BtNode* node, const long int position);

/* ************************************************************************* */

#endif /* BTREE_H_ */
