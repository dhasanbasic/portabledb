/*
 * test.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include <stdio.h>

#include "btree.h"

void PrepareNewDb()
{
	FILE* file;

	remove("db.dat");
	file = fopen("db.dat","wb");
	fclose(file);
}

int main(void)
{
	FILE* file;
	BtTree* tree;

	printf("PortableDB Version 0.1\n------------------------\n\n");

	PrepareNewDb();

	file = fopen("db.dat","r+b");

	tree = CreateTree(file,2,16,0,4,2);

	printf("position\t\t: %ld\n", tree->position);
	printf("freelistPosition\t: %ld\n", tree->meta->freelistPosition);
	printf("rootPosition\t\t: %ld\n", tree->meta->rootPosition);

	printf("nodeLength\t\t: %u\n", tree->nodemeta->nodeLength);
	printf("minRecords\t\t: %u\n", tree->nodemeta->minRecords);
	printf("maxRecords\t\t: %u\n", tree->nodemeta->maxRecords);
	printf("posChildren\t\t: %u\n", tree->nodemeta->posChildren);
	printf("posLeaf\t\t\t: %u\n", tree->nodemeta->posLeaf);
	printf("posCount\t\t: %u\n", tree->nodemeta->posCount);

	printf("u. short\t\t: %u\n", sizeof(unsigned short int));
	printf("long int\t\t: %u\n", sizeof(long int));


	fclose(file);

	return 0;
}
