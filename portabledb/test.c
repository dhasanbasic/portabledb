/*
 * test.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include <stdio.h>
#include <malloc.h>

#include <string.h>

#include "btree/btree.h"

void PrintTree(const BtNode* root)
{
	SHORT i,j;
	BtNode* tmp = AllocateNode(root->tree,MODE_MEMORY);
	char* record = (char*)malloc(root->tree->meta->recordLength);

	printf("p:%-4lu  L:%u  R:%-2u ", root->position, GetLeaf(root), GetCount(root));

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
	tree = CreateTree(file,3,1,0,1,6);
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

LONG createNode(
		const char* d,
		const LONG c1,
		const LONG c2,
		const LONG c3,
		const LONG c4,
		const LONG c5,
		const LONG c6,
		const SHORT leaf,
		const SHORT count,
		const LONG position,
		FILE* file)
{
	LONG pos;
	char* data = (char*)malloc(33);

	if(position == 0)
	{
		fseek(file,0,SEEK_END);
		pos = ftell(file);
	}
	else pos = position;

	memcpy(data,	d,		5);
	memcpy(data+5,	&c1,	4);
	memcpy(data+9,	&c2,	4);
	memcpy(data+13,	&c3,	4);
	memcpy(data+17,	&c4,	4);
	memcpy(data+21,	&c5,	4);
	memcpy(data+25,	&c6,	4);
	memcpy(data+29,	&leaf,	2);
	memcpy(data+31,	&count,	2);

	fseek(file,pos,SEEK_SET);
	fwrite(data,33,1,file);
	free(data);
	return pos;
}


int main(void)
{
	BtTree* tree;

	printf("PortableDB Version 0.1\n------------------------\n\n");

//	PrepareNewDb();

	tree = (BtTree*)malloc(sizeof(BtTree));

	tree->file = fopen("db.dat","r+b");
	tree->position = 0;

	ReadTree(tree);

/*                            BEGIN - TESTS                           */
/*
	createNode(
			"P\0\0\0\0",
			createNode(
					"CGM\0\0",
					createNode("AB\0\0\0",0,0,0,0,0,0,LEAF,2,0,tree->file),
					createNode("DEF\0\0",0,0,0,0,0,0,LEAF,3,0,tree->file),
					createNode("JKL\0\0",0,0,0,0,0,0,LEAF,3,0,tree->file),
					createNode("NO\0\0\0",0,0,0,0,0,0,LEAF,2,0,tree->file),
					0,0,INTERNAL,3,0,tree->file),
			createNode(
					"TX\0\0\0",
					createNode("QRS\0\0",0,0,0,0,0,0,LEAF,3,0,tree->file),
					createNode("UV\0\0\0",0,0,0,0,0,0,LEAF,2,0,tree->file),
					createNode("YZ\0\0\0",0,0,0,0,0,0,LEAF,2,0,tree->file),
					0,0,0,INTERNAL,2,0,tree->file),
			0,0,0,0,INTERNAL,1,44,tree->file);
*/

	PrintTree((BtNode*)tree->root);

/*                             END - TESTS                            */

	/* deallocate the tree */
	WriteTree(tree);
	FreeTree(tree);

	return 0;
}

// void main(void) { PrepareNewDb(); }
