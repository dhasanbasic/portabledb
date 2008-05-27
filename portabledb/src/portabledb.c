
#include <stdio.h>

#include "btreelayer.h"

void prepareNew()
{
	BTREE* btree = allocateBtree();
	FILE* file;
	
	remove("db.dat");
	file = fopen("db.dat", "w+b");
	file = freopen("db.dat","r+b",file);
	
	createBtree(btree, file, 2, 8, 0, 4);
	freeBtree(btree);
	fclose(file);
}

int main(void)
{
//	prepareNew();
	
	FILE* file = fopen("db.dat","r+b");
	BTREE* btree = allocateBtree();
	
	btree->file = file;
	btree->position = 0;
	readBtree(btree);
	
	printf("%d\n", BtreeSearch(btree, "Dink", NULL));

	freeBtree(btree);
	fclose(file);

	return 0;
}
