
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btreelayer.h"

/*
void printNode(NODE* node)
{
	UINT i;
	
	printf("\n--> records = %d", node->records);
	
	printf("\n--> DATA: ");
	
	for(i=0; i<(node->btree->tag->rSize * node->btree->tag->order); i++)
		printf("%c",(node->data[i]!=0)?node->data[i]:'?');
	
	printf("\n--> SUBNODES: ");
	
	for(i=0; i<(node->btree->tag->order+1); i++)
		printf("%d ", node->subnodes[i]);
	
	printf("\n");
}
*/
void prepareNew()
{
	BTREE* btree;	
	NODE* node;
	FILE* file;

	remove("db.dat");
	file = fopen("db.dat", "w+b");
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
//	BTREE* btree = createBtree();
	/******************************************************/
//	p->record = (char*)malloc(4);
//	btree->file = fopen("db.dat", "r+b");
//	btree->position = 0;
	/******************************************************/
	
	prepareNew();
	
	/******************************************************/
//	fclose(btree->file);
	/******************************************************/
	return EXIT_SUCCESS;
}


