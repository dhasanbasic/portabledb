
#include "btree.h"
#include "node.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

BTREE* createBtree()
{
	BTREE* btree = (BTREE*)malloc(sizeof(BTREE));
	btree->tag = (BTAG*)malloc(sizeof(BTAG));
	return btree;
}

void freeBtree(BTREE* btree)
{
	free(btree->tag);
	free(btree);
}

void loadTree(BTREE* btree)
{
	fseek(btree->file, btree->position, SEEK_SET);
	fread(btree->tag, sizeof(BTAG), 1, btree->file);
}

void saveTree(BTREE* btree)
{
	fseek(btree->file, btree->position, SEEK_SET);
	fwrite(btree->tag, sizeof(BTAG), 1, btree->file);
}

int recursiveSearch(BPARAM* p)
{
	UINT keyLeft, keyRight, i;
	int comparsion;
	
	BTAG* tag = p->node->btree->tag;
	
	/* location of the keys for the first and last record */
	keyLeft = tag->kPos;
	keyRight = (p->node->records-1) * tag->rSize + tag->kPos;
	
	/* left subnode ? */
	if (memcmp(p->key, &p->node->data[keyLeft], tag->kSize) < 0)
	{
		p->node->position = p->node->subnodes[0];

		if (p->node->position > 0)
		{
			loadNode(p->node);
			recursiveSearch(p);
		}
		
		p->position = 0;
		return -1;
	}

	/* right subnode ? */	
	if (memcmp(p->key, &p->node->data[keyRight], tag->kSize) > 0)
	{
		p->node->position = p->node->subnodes[p->node->records];

		if (p->node->position > 0)
		{
			loadNode(p->node);
			recursiveSearch(p);
		}
		
		p->position = p->node->records;
		return -1;
	}

	/* inside of the node? */
	for(i=0; i < p->node->records; i++)
	{
		keyLeft = i * tag->rSize + tag->kPos;
		keyRight = (i+1) * tag->rSize + tag->kPos;

		/* is this the searched record? */
		comparsion = memcmp(p->key, &p->node->data[keyLeft], tag->kSize);
		
		if(comparsion == 0)
		{
			p->position = i;
			memcpy(p->record, &p->node->data[keyLeft-tag->kSize], tag->rSize);
			return 1;
		}
		
		/* skip "between" comparsion for the last record */
		if(i == p->node->records-1) continue;
		
		/* is the record in the subnode between keyLeft and keyRight? */
		if(comparsion > 0 &&
				memcmp(p->key, &p->node->data[keyRight], tag->kSize) < 0)
		{
			p->node->position = p->node->subnodes[i+1];

			if (p->node->position > 0)
			{
				loadNode(p->node);
				recursiveSearch(p);
			}
			
			p->position = i;
			return -1;
		}
	}
	return -1;
}

void insertRecord(BPARAM* p)
{
	int result;
	BTAG* tag = p->node->btree->tag;

	/* if the root node is empty, just insert the record */
	if(p->node->records == 0)
	{
		memcpy(&p->node->data[0], p->record, tag->rSize);
		p->node->records++;
		
		printf("(**) record successfully inserted [%d]\n", 0);
		return;
	}

	/* search for a place */
	p->key = &p->record[tag->kPos];	
	result = recursiveSearch(p);
	
	/* the record already exists */
	if(result > 0)
	{
		printf("(EE) record already exists [%d]\n", p->position);
		return;
	}
	
	/* the record will be inserted at p->position */
	if(p->position < p->node->records)
	{
		/* shift the records, if needed */
		memmove(&p->node->data[(p->position + 1) * tag->rSize],
				&p->node->data[p->position * tag->rSize],
				(p->node->records - p->position) * tag->rSize);
		
		/* shift the subnodes, if needed */
		memmove(&p->node->subnodes[p->position + 2],
				&p->node->subnodes[p->position + 1],
				(p->node->records - p->position) * sizeof(LONG));		
	}
	
	memcpy(&p->node->data[p->position * tag->rSize], p->record, tag->rSize);
	p->node->subnodes[p->position + 1] = 0;
	p->node->records++;
	printf("(**) record successfully inserted [%d]\n", p->position);
}
