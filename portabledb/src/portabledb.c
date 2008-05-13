
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "node.h"

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

void prepareNew(FILE* file)
{
	BTREE* btree = createBtree();
	NODE* node;
	
	btree->file = file;
	btree->position = 0;
	btree->tag->root = sizeof(BTREE);
	btree->tag->order = 3;
	btree->tag->rSize = 4;
	btree->tag->kPos = 0;
	btree->tag->kSize = 2;
	
	node = createNode(btree);
	
	node->position = btree->tag->root;
	
	saveTree(btree);
	saveNode(node);
	freeNode(node);
	freeBtree(btree);
}

int main(void)
{
	BPARAM* p = (BPARAM*)malloc(sizeof(BPARAM));
	BTREE* btree = createBtree();
	NODE* node;
	/******************************************************/
	p->record = (char*)malloc(4);
	btree->file = fopen("db.dat", "r+b");
	btree->position = 0;
	/******************************************************/
/*
	system("rm db.dat");
	system("touch db.dat");
	btree->file = fopen("db.dat", "r+b");
	prepareNew(btree->file);
*/
	loadTree(btree);
	
	node = createNode(btree);
	node->position = btree->tag->root;
	
	loadNode(node);
	p->node = node;
	
	strncpy(p->record, "aa00", 4);
	insertRecord(p);

	strncpy(p->record, "bb00", 4);
	insertRecord(p);

	printNode(node);
	
	/******************************************************/
	fclose(btree->file);
	freeNode(node);
	freeBtree(btree);
	free(p->record);
	free(p);
	/******************************************************/
	return EXIT_SUCCESS;
}
