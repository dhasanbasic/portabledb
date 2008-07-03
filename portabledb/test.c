/*
 * test.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include <stdio.h>
#include <malloc.h>

#include "btree.h"

void PrepareNewDb()
{
	FILE* file;
	BtTree* tree;

	remove("db.dat");
	file = fopen("db.dat","wb");
	file = freopen("db.dat", "r+b", file);
	tree = CreateTree(file,2,1,0,1,2);
	fclose(file);
}

void PrintTreeMeta(const BtTree* tree)
{
	printf("position\t\t: %ld\n", tree->position);
	printf("freelistPosition\t: %ld\n", tree->meta->freelistPosition);
	printf("rootPosition\t\t: %ld\n", tree->meta->rootPosition);

	printf("nodeLength\t\t: %u\n", tree->nodeLength);
	printf("minRecords\t\t: %u\n", tree->minRecords);
	printf("maxRecords\t\t: %u\n", tree->maxRecords);
	printf("posChildren\t\t: %u\n", tree->posChildren);
	printf("posLeaf\t\t\t: %u\n", tree->posLeaf);
	printf("posCount\t\t: %u\n", tree->posCount);
}

int main(void)
{
	BtTree* tree;
	BtNode* root;

	printf("PortableDB Version 0.1\n------------------------\n\n");

	tree = (BtTree*)malloc(sizeof(BtTree));

	tree->file = fopen("db.dat","r+b");
	tree->position = 0;

	ReadTree(tree);
	root = (BtNode*)tree->root;

	/*                            BEGIN - TESTS                           */

	/*                             END - TESTS                            */

	/* deallocate the tree */
	free((BtNode*)root->data);
	free(root);
	free(tree->meta);
	free(tree->freelist);
	free(tree);

	fclose(tree->file);

	return 0;
}

// void main(void) { PrepareNewDb(); }
