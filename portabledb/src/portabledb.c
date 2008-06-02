
#include <stdio.h>
#include <malloc.h>

#include "btreelayer.h"

void prepareNew()
{
	FILE* file;
	BTREEDESC* desc = (BTREEDESC*)malloc(sizeof(BTREEDESC));
	BTREE* btree;
	
	remove("db.dat");
	file = fopen("db.dat", "w+b");
	fwrite("PortableDB Alpha", 16, 1, file);
	file = freopen("db.dat","r+b",file);
	
	desc->minChildren = 2;
	desc->recordSize = 4;
	desc->keyOffset = 0;
	desc->keySize = 2;
	
	btree = createBtree(file, desc);

	printf("    rootPtr: %d\n", btree->desc->btreedesc->rootPtr);
	printf(" minRecords: %d\n", btree->desc->minRecords);
	printf(" maxRecords: %d\n", btree->desc->maxRecords);
	printf("   nodeSize: %d\n", btree->desc->nodeSize);
	printf("    records: %d\n", btree->desc->recordsOffset);
	printf("   children: %d\n", btree->desc->childrenOffset);
	printf("      count: %d\n", btree->desc->countOffset);
	printf("       leaf: %d\n", btree->desc->leafOffset);

	freeBtree(btree);
	
	fclose(file);
}

int main(void)
{
	FILE* file;
	BTREE* btree;
	
//	prepareNew();
	
	file = fopen("db.dat","r+b");

	btree = readBtree(file,16);
	
	printf("records: %u\n", getCount(btree->root));
	printf("leaf:    %u\n", getLeaf(btree->root));
	
	freeBtree(btree);
	fclose(file);
	
	return 0;
}
