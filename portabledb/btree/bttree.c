/*
 * btree.c
 *
 *  Created on: Jul 1, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */

#include "../btree.h"

#include <stdio.h>
#include <string.h>

BtTree*	CreateTree(
			FILE*		file,
			const SHORT	order,
			const SHORT	recordLength,
			const SHORT	keyPosition,
			const SHORT	keyLength,
			const SHORT	freelistSize)
{
	BtTree* tree;
	BtNode* root;

	/* allocate the meta-data structures */
	tree = (BtTree*)malloc(sizeof(BtTree));
	tree->meta = (BtMeta*)malloc(sizeof(BtMeta));

	/* fill in the B-tree meta-data */
	tree->meta->order			= order;
	tree->meta->recordLength	= recordLength;
	tree->meta->keyPosition		= keyPosition;
	tree->meta->keyLength		= keyLength;
	tree->meta->freelistSize	= freelistSize;
	tree->meta->freeNodes		= 0;

	/* fill in the node meta-data */
	CalculateNodeMeta(tree);
	tree->freelist = (LONG*)malloc(freelistSize * sizeof(LONG));

	/* allocate the root node */
	root = AllocateNode(tree,MODE_MEMORY);

	/* fill in the root node */
	SetCount(root, 0);
	SetLeaf(root, LEAF);

	/* determine the positions of the structures in the file */
	tree->file = file;
	fseek(file,0,SEEK_END);

	tree->position = ftell(file);
	tree->meta->freelistPosition = tree->position + sizeof(BtMeta);
	tree->meta->rootPosition = tree->position + sizeof(BtMeta)
		+ freelistSize*sizeof(LONG);
	root->position = tree->meta->rootPosition;

	/* save the structures */
	fwrite(tree->meta,sizeof(BtMeta),1,file);
	fwrite(tree->freelist, sizeof(LONG), freelistSize, file);
	fwrite(root->data, tree->nodeLength, 1, file);

	tree->root = root;

	return tree;
}


void	WriteTree(BtTree* tree)
{
	fseek(tree->file, tree->position, SEEK_SET);
	fwrite(tree->meta,sizeof(BtMeta),1,tree->file);

	fseek(tree->file, tree->meta->freelistPosition, SEEK_SET);
	fwrite(tree->freelist, sizeof(LONG),
			tree->meta->freelistSize, tree->file);
}

BtTree*	ReadTree(FILE* file, const LONG position)
{
	BtTree* tree;
	BtNode* root;

	/* allocate the meta-data structures */
	tree = (BtTree*)malloc(sizeof(BtTree));
	tree->position = position;
	tree->file = file;

	/* fill in the meta-data structures */
	tree->meta		= (BtMeta*)malloc(sizeof(BtMeta));
	fseek(file, position, SEEK_SET);
	fread(tree->meta,sizeof(BtMeta),1,file);

	CalculateNodeMeta(tree);

	/* create and fill the free list */
	tree->freelist = (LONG*) malloc(tree->meta->freelistSize*sizeof(LONG));

	fseek(file, tree->meta->freelistPosition, SEEK_SET);
	fread(tree->freelist, sizeof(LONG), tree->meta->freelistSize, file);

	/* create and fill in the root node */
	root			= (BtNode*)malloc(sizeof(BtNode));
	root->data		= (char*)malloc(tree->nodeLength);
	root->tree		= tree;
	root->position	= tree->meta->rootPosition;

	fseek(file, tree->meta->rootPosition, SEEK_SET);
	fread(root->data, tree->nodeLength, 1, file);

	tree->root = root;
	return tree;
}

void FreeTree(BtTree* tree)
{
	BtNode* root = (BtNode*)tree->root;

	free(root->data);
	free(root);
	free(tree->meta);
	free(tree->freelist);
	free(tree);
}

int	InsertRecord(BtTree* tree, const void* record)
{
	char* space = (char*)malloc(tree->meta->keyLength);

	if(SearchRecord(tree,record+tree->meta->keyPosition,space)
			== SEARCH_NOTFOUND)
	{
		free(space);
		BtreeInsert(tree, record);
		return INSERTION_SUCCEEDED;
	}
	free(space);
	return INSERTION_FAILED;
}

int		SearchRecord(BtTree* tree, const void* key, void* record)
{
	BtSearchResult* p = (BtSearchResult*)malloc(sizeof(BtSearchResult));
	BtNode* tmp = 0;
	int result;

	p->result = SEARCH_NOTFOUND;

	BtreeSearch((BtNode*)tree->root,key,p);

	if (p->result == SEARCH_FOUND)
	{
		if(p->position != tree->meta->rootPosition)
		{
			tmp = AllocateNode(tree,MODE_MEMORY);
			tmp->position = p->position;
			ReadNode(tmp);
		}
		else
			tmp = (BtNode*)tree->root;

		memcpy(record, GetRecord(tmp,p->index), tree->meta->recordLength);
	}

	if (tmp > 0 && tmp->position != tree->meta->rootPosition)
	{
		free(tmp->data);
		free(tmp);
	}

	result = p->result;
	free(p);

	return result;
}

int		DeleteRecord(BtTree* tree, const void* key)
{
	BtNode* root = (BtNode*)tree->root;
	BtNode* tmp;
	int result;

	char* space = (char*)malloc(tree->meta->keyLength);

	if(SearchRecord(tree,key,space)== SEARCH_NOTFOUND)
	{
		free(space);
		return DELETION_FAILED;
	}

	result = BtreeDelete(root,key);

	if(GetCount(root) == 0)
	{
		/* decrease the height of the B-tree */
		tmp = AllocateNode(tree,MODE_MEMORY);
		tmp->position = GetChild(root,1);
		ReadNode(tmp);
		tree->meta->rootPosition = tmp->position;
		tree->root = tmp;

		/* free the child z */
		if(tree->meta->freeNodes < tree->meta->freelistSize)
		{
			tree->freelist[tree->meta->freeNodes] = root->position;
			tree->meta->freeNodes++;
		}

		free(root->data);
		free(root);

		printf("*** B-tree height decreased\n");
	}
	return result;
}
