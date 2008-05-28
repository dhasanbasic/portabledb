
#include <stdio.h>
#include <string.h>

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
	int result;
	char record[8];
	char* key = "XSSS";
	
//	prepareNew();
	
	FILE* file = fopen("db.dat","r+b");

	BTREE* btree = TreeRead(file, 0);
	
	memcpy(btree->root->records, "A_RecordB_RecordC_Record", 24);
	COUNT(btree->root) = 3;
	
	result = TreeSearch(btree, key, record);
	
	if(result == SEARCH_FOUND)
		printf("Record[%s] was found: %s\n", key, record);
	else
		printf("Record[%s] was not found!\n", key);
	
	TreeFree(btree);
	fclose(file);
	return 0;
}
