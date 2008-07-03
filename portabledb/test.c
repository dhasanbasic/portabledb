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
	tree = CreateTree(file,2,16,0,4,2);
	fclose(file);
}

void PrintTreeMeta(const BtTree* tree)
{
	printf("position\t\t: %ld\n", tree->position);
	printf("freelistPosition\t: %ld\n", tree->meta->freelistPosition);
	printf("rootPosition\t\t: %ld\n", tree->meta->rootPosition);

	printf("nodeLength\t\t: %u\n", tree->nodemeta->nodeLength);
	printf("minRecords\t\t: %u\n", tree->nodemeta->minRecords);
	printf("maxRecords\t\t: %u\n", tree->nodemeta->maxRecords);
	printf("posChildren\t\t: %u\n", tree->nodemeta->posChildren);
	printf("posLeaf\t\t\t: %u\n", tree->nodemeta->posLeaf);
	printf("posCount\t\t: %u\n", tree->nodemeta->posCount);
}

int main(void)
{
	BtTree* tree;

	printf("PortableDB Version 0.1\n------------------------\n\n");

	tree = (BtTree*)malloc(sizeof(BtTree));

	tree->file = fopen("db.dat","r+b");
	tree->position = 0;

	ReadTree(tree);

	/*                            BEGIN - TESTS                           */



	/*                             END - TESTS                            */

	/* deallocate the tree */
	free(tree->root->data);
	free(tree->root);
	free(tree->meta);
	free(tree->nodemeta->freelist);
	free(tree->nodemeta);
	free(tree);

	fclose(tree->file);

	return 0;
}
