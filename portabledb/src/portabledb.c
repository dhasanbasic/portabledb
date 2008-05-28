
#include <stdio.h>

#include "btreelayer.h"

void prepareNew()
{
	BTREE* btree;
	FILE* file;
	
	remove("db.dat");
	file = fopen("db.dat", "w+b");
	file = freopen("db.dat","r+b",file);
	
	btree = TreeCreate(file, 2, 8, 0, 4);
	
	TreeWrite(btree);
	NodeWrite(btree->root);
	
	TreeFree(btree);
	fclose(file);
}

int main(void)
{
//	prepareNew();
	
	FILE* file = fopen("db.dat","r+b");

	BTREE* btree = TreeRead(file, 0);
	
	//
	
	TreeFree(btree);
	fclose(file);
	return 0;
}
