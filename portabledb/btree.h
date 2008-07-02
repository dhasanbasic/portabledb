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
/*                        B-tree functions structures                        */
/* ************************************************************************* */

BtTree*	CreateTree(
			FILE*						file,
			const unsigned short int	order,
			const unsigned short int	recordLength,
			const unsigned short int	keyPosition,
			const unsigned short int	keyLength,
			const unsigned short int	freelistSize);

void	WriteTree(
			const BtTree*	tree,
			const long int	position);

void	ReadTree(
			const BtTree*	tree,
			const long int	position);

/* ************************************************************************* */


#endif /* BTREE_H_ */
