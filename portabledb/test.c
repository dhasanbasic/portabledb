/*
 * test.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include <stdio.h>
#include <malloc.h>

#include <string.h>

#include "btree.h"

void PrintTree(const BtNode* root)
{
	SHORT i,j;
	BtNode* tmp = AllocateNode(root->tree,MODE_MEMORY);
	char* record = (char*)malloc(root->tree->meta->recordLength);

	printf("p[%lu] L[%u] R[%u]  ", root->position, GetLeaf(root), GetCount(root));

	for(i=1; i <= GetCount(root); i++)
	{
		memcpy(record, GetRecord(root,i), root->tree->meta->recordLength);
		printf("[");
		for(j=0;j<root->tree->meta->recordLength;printf("%c",record[j++]));
		printf("]");
	}

	if(GetLeaf(root) == INTERNAL)
	{
		printf(" ");
		/* print the children list */
		for(i=1; i <= GetCount(root)+1; i++)
			printf("{%lu}",GetChild(root,i));
		printf("\n");

		for(i=1; i <= GetCount(root)+1; i++)
		{
			tmp->position = GetChild(root,i);
			ReadNode(tmp);
			PrintTree(tmp);
		}
	}
	printf("\n");

	free(tmp->data);
	free(tmp);
	free(record);
}

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

	printf("PortableDB Version 0.1\n------------------------\n\n");

	tree = (BtTree*)malloc(sizeof(BtTree));

	tree->file = fopen("db.dat","r+b");
	tree->position = 0;

	ReadTree(tree);

/*                            BEGIN - TESTS                           */

	InsertRecord(tree,"Y");
	InsertRecord(tree,"A");
	InsertRecord(tree,"F");
	InsertRecord(tree,"G");
	InsertRecord(tree,"B");
	InsertRecord(tree,"M");
	InsertRecord(tree,"L");
	InsertRecord(tree,"K");
	InsertRecord(tree,"X");
	InsertRecord(tree,"I");
	InsertRecord(tree,"Z");
	InsertRecord(tree,"J");
	InsertRecord(tree,"V");
	InsertRecord(tree,"C");
	InsertRecord(tree,"H");

	PrintTree((BtNode*)tree->root);

/*                             END - TESTS                            */

	/* deallocate the tree */
	free(((BtNode*)tree->root)->data);
	free((BtNode*)tree->root);
	free(tree->meta);
	free(tree->freelist);
	free(tree);

	fclose(tree->file);

	return 0;
}

// void main(void) { PrepareNewDb(); }
