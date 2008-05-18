
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btreelayer.h"


void printNode(NODE* node)
{
	SINT i;
	
	printf(" records: %d\n", MRECORDS(node));
	printf("    data: ");
	
	for(i=0; i<(MORDER(node->btree)*MRECSIZE(node->btree)); i++)
		printf("%c",(MDATA(node)[i]!=0)?MDATA(node)[i]:'?');
	
	printf("\nsubnodes: ");
	
	for(i=0; i<(MORDER(node->btree)+1); i++)
		printf("%d ", MSUBNODES(node)[i]);
	
	printf("\n");
}

void prepareNew()
{
	BTREE* btree;	
	NODE* node;
	FILE* file;

	remove("db.dat");
	file = fopen("db.dat", "w+b");
	
	fputs("PDB", file);
	
	file = freopen("db.dat","r+b",file);
	
	btree = allocateBtree();
	createBtree(btree, file, 3 , 4, 0, 2);
	saveBtree(btree);
	
	fseek(file,0,SEEK_END);
	btree->tag->rootPosition = ftell(file);
	saveBtree(btree);
	
	node = allocateNode(btree);
	saveNode(node);
	
	fclose(file);
	freeNode(node);
	freeBtree(btree);
}

int main(void)
{
	BTREE* btree = allocateBtree();
	NODE* node;

	btree->file = fopen("db.dat","r+b");
	btree->position = 3;
	loadBtree(btree);

	node = allocateNode(btree);
	loadNode(node);
	printNode(node);

	freeNode(node);
	fclose(btree->file);
	freeBtree(btree);
	
	return EXIT_SUCCESS;
}


