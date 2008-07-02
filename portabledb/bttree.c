/*
 * btree.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include "btree.h"

#include <malloc.h>
#include <stdio.h>

BtTree*	CreateTree(
			FILE*						file,
			const unsigned short int	order,
			const unsigned short int	recordLength,
			const unsigned short int	keyPosition,
			const unsigned short int	keyLength,
			const unsigned short int	freelistSize)
{
	BtTree*  tree;

	/* allocate the meta-data structures */
	tree = (BtTree*)malloc(sizeof(BtTree));
	tree->meta = (BtMeta*)malloc(sizeof(BtMeta));
	tree->nodemeta = (BtNodeMeta*)malloc(sizeof(BtNodeMeta));

	/* fill in the B-tree meta-data */
	tree->meta->order			= order;
	tree->meta->recordLength	= recordLength;
	tree->meta->keyPosition		= keyPosition;
	tree->meta->keyLength		= keyLength;
	tree->meta->freelistSize	= freelistSize;
	tree->meta->freeNodes		= 0;

	/* fill in the node meta-data */
	CalculateNodeMeta(tree->meta, tree->nodemeta);

	/* allocate the free nodes list */
	tree->freelist = (long int*)malloc(freelistSize*sizeof(long int));

	/* allocate the root node */
	tree->root			= (BtNode*)malloc(sizeof(BtNode));
	tree->root->data	= (char*)malloc(tree->nodemeta->nodeLength);

	/* fill in the root node */
	tree->root->meta	= tree->nodemeta;
	SetCount(tree->root, 0);
	SetLeaf(tree->root, CHILD);

	/* determine the positions of the structure in the file */
	tree->file = file;
	fseek(file,0,SEEK_END);

	tree->position = ftell(file);
	tree->meta->freelistPosition = tree->position + sizeof(BtMeta);
	tree->meta->rootPosition = tree->position + sizeof(BtMeta)
		+ freelistSize*sizeof(long int);

	/* save the structures */
	fwrite(tree->meta,sizeof(BtMeta),1,file);
	fwrite(tree->freelist, sizeof(long int), freelistSize, file);
	fwrite(tree->root->data, tree->nodemeta->nodeLength, 1, file);

	return tree;
}
/*
void	WriteTree(
			const BtTree*	tree,
			const long int	position);

void	ReadTree(
			const BtTree*	tree,
			const long int	position);
*/
