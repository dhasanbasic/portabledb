/*
 * bthelper.c
 *
 *  Created on: Jul 6, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */
#include "../btree.h"

#include <string.h>

void	BtreeSearch(
			BtNode* node,
			const char* key,
			BtSearchResult* p)
{
	SHORT i = 1;
	SHORT keyLength = node->tree->meta->keyLength;
	BtNode* next;

	while (i <= GetCount(node)
		&& (memcmp(key, GetKey(node,i), keyLength) > 0)) i++;

	if (i <= GetCount(node)
		&& (memcmp(key, GetKey(node,i), keyLength) == 0))
	{
		p->result = SEARCH_FOUND;
		p->position = node->position;
		p->index = i;
		goto END;
	}

	if (GetLeaf(node) == LEAF)
	{
		p->result = SEARCH_NOTFOUND;
		goto END;
	}
	else
	{
		next = AllocateNode(node->tree,MODE_MEMORY);
		next->position = GetChild(node,i);
		ReadNode(next);
		BtreeSearch(next,key,p);
	}

END:
	if(node->position != node->tree->meta->rootPosition)
	{
		free(node->data);
		free(node);
	}
}

void	BtreeInsert(BtTree* tree, const void* record)
{
	BtNode* root = (BtNode*)tree->root;
	BtInsertParam* p = (BtInsertParam*)malloc(sizeof(BtInsertParam));

	p->record	= (char*)record;
	p->key		= p->record + tree->meta->keyPosition;

	if(GetCount(root) == tree->maxRecords)
	{
		/* replace the root by a new empty root with one child */
		BtNode* newRoot = AllocateNode(tree,MODE_FREELIST);
		tree->root = newRoot;
		SetLeaf(newRoot,INTERNAL);
		SetCount(newRoot,0);
		SetChild(newRoot,1,root->position);

		/* split the old root */
		BtreeSplitChild(newRoot,1,root);
		tree->meta->rootPosition = newRoot->position;
		free(root->data);
		free(root);
		WriteTree(tree);

		/* continue the insertion */
		p->node = newRoot;
		BtreeInsertNonfull(p);
	}
	else
	{
		p->node = root;
		BtreeInsertNonfull(p);
	}
}

void	BtreeSplitChild(
			BtNode* x,
			const SHORT i,
			BtNode* y)
{
	BtTree* tree		= x->tree;
	BtNode* z			= AllocateNode(tree,MODE_FREELIST);
	SHORT order			= tree->meta->order;
	SHORT recordLength	= tree->meta->recordLength;
	SHORT minRecords	= tree->minRecords;

	/* update the new child node (z) */
	SetLeaf(z, GetLeaf(y));
	SetCount(z, minRecords);

	memcpy(GetRecord(z,1), GetRecord(y,order+1), minRecords * recordLength);

	if (GetLeaf(y) == INTERNAL)
	{
		memcpy(GetChildPtr(z,1), GetChildPtr(y,order+1), order * sizeof(LONG));
	}

	WriteNode(z);

	/* update the split child node */
	SetCount(y, minRecords);
	WriteNode(y);

	/* update the parent node */
	memmove(GetChildPtr(x,i+2), GetChildPtr(x,i+1),
			(GetCount(x)-i+1)*sizeof(LONG));

	memmove(GetRecord(x,i+1), GetRecord(x,i),
			(GetCount(x)-i+1)*recordLength);

	SetChild(x,i+1,z->position);
	memcpy(GetRecord(x,i), GetRecord(y,order), recordLength);

	SetCount(x, GetCount(x)+1);

	WriteNode(x);
}

void	BtreeInsertNonfull(BtInsertParam* p)
{
	SHORT i = GetCount(p->node);
	SHORT keyLength = p->node->tree->meta->keyLength;
	SHORT recordLength = p->node->tree->meta->recordLength;
	BtNode* child;


	if(GetLeaf(p->node) == LEAF)
	{
		/* shift a few records (if needed to) */
		while (i>=1 && (memcmp(p->key, GetKey(p->node,i), keyLength) < 0))
		{
			memcpy(GetRecord(p->node,i+1),GetRecord(p->node,i),recordLength);
			i--;
		}

		/* insert the record and update the node */
		memcpy(GetRecord(p->node,i+1), p->record, recordLength);
		SetCount(p->node,GetCount(p->node)+1);
		WriteNode(p->node);
	}
	else
	{
		/* find the appropriate child node */
		while (i>=1 && (memcmp(p->key, GetKey(p->node,i), keyLength) < 0))
			i--;
		/* traverse to the right child node of the i-th record */
		i++;

		child = AllocateNode(p->node->tree,MODE_MEMORY);
		child->position = GetChild(p->node,i);
		ReadNode(child);

		/* is it a full child node? */
		if(GetCount(child) == p->node->tree->maxRecords)
		{
			BtreeSplitChild(p->node,i,child);
			if(memcmp(p->key, GetKey(p->node,i), keyLength) > 0)
			{
				i++;
				child->position = GetChild(p->node,i);
				ReadNode(child);
			}
		}

		/* free unnecessary allocated resources */
		if(p->node->position != p->node->tree->meta->rootPosition)
		{
			free(p->node->data);
			free(p->node);
		}

		/* recursion */
		p->node = child;
		BtreeInsertNonfull(p);
	}
}

void	BtreeMergeNodes(
			BtNode* x,
			BtNode* y,
			const SHORT i,
			BtNode* z)
{
	BtTree* tree		= x->tree;
	SHORT order			= tree->meta->order;
	SHORT recordLength	= tree->meta->recordLength;
	SHORT minRecords	= tree->minRecords;
	SHORT maxRecords	= tree->maxRecords;

	/* copy the median key from parent x to child y */
	memcpy(GetRecord(y,order), GetRecord(x,i), recordLength);

	/* copy the keys and children (if necessary) of child z to child y */
	memcpy(GetRecord(y,order+1), GetRecord(z,1), minRecords * recordLength);

	if (GetLeaf(z) == INTERNAL)
		memcpy(GetChildPtr(y,order+1), GetChildPtr(z,1), order * sizeof(LONG));

	SetCount(y,maxRecords);
	WriteNode(y);

	/* remove both the median key and pointer to child z from x */
	memmove(GetRecord(x,i), GetRecord(x,i+1), (GetCount(x)-i)*recordLength);

	memmove(GetChildPtr(x,i+1), GetChildPtr(x,i+2),
			(GetCount(x)-i)*sizeof(LONG));

	SetCount(x,GetCount(x)-1);
	WriteNode(x);

	/* free the child z */
	if(tree->meta->freeNodes < tree->meta->freelistSize)
	{
		tree->freelist[tree->meta->freeNodes] = z->position;
		tree->meta->freeNodes++;
	}

	free(z->data);
	free(z);
}

void	BtreeReduceSibling(
			BtNode* parent,
			BtNode* child,
			BtNode* sibling,
			const SHORT i,
			const int type)
{
	SHORT recordLength = parent->tree->meta->recordLength;

	if(type == TYPE_LEFT)
	{
		/* allocate space for the new record */
		memmove(GetRecord(child,2),GetRecord(child,1),
				GetCount(child)*recordLength);

		memmove(GetChildPtr(child,2),GetChildPtr(child,1),
				(GetCount(child)+1)*sizeof(LONG));

		/* take the predecessor of the child node from the parent */
		memcpy(GetRecord(child,1), GetRecord(parent,i-1), recordLength);

		/* push the last record from the left sibling up to the parent */
		memcpy(GetRecord(parent,i-1),
				GetRecord(sibling,GetCount(sibling)),
				recordLength);

		/* update the child pointers of the siblings */
		memcpy(GetChildPtr(child,1),
				GetChildPtr(sibling,GetCount(sibling)+1),
				sizeof(LONG));

		SetCount(sibling,GetCount(sibling)-1);
	}
	else /* type = TYPE_RIGHT */
	{
		/* take the successor of the child node from the parent */
		memcpy(GetRecord(child,GetCount(child)+1),
				GetRecord(parent,i), recordLength);

		/* push the last record from the right sibling up to the parent */
		memcpy(GetRecord(parent,i),
				GetRecord(sibling,1),
				recordLength);

		/* update the child pointers of the siblings */
		memcpy(GetChildPtr(child,GetCount(child)+2),
				GetChildPtr(sibling,1),sizeof(LONG));

		/* update the sibling */
		SetCount(sibling,GetCount(sibling)-1);

		memmove(GetRecord(sibling,1),GetRecord(sibling,2),
				GetCount(sibling)*recordLength);

		memmove(GetChildPtr(sibling,1),GetChildPtr(sibling,2),
				(GetCount(sibling)+1)*sizeof(LONG));
	}

	/* save the changes */
	SetCount(child,GetCount(child)+1);
	WriteNode(parent);
	WriteNode(sibling);
	WriteNode(child);
}

int		BtreeDelete(BtNode* node, const void* key)
{
	SHORT i = 1;
	SHORT order = node->tree->meta->order;
	SHORT keyLength = node->tree->meta->keyLength;
	SHORT recordLength = node->tree->meta->recordLength;
	SHORT minRecords = node->tree->minRecords;

	BtNode *y, *z, *c;

	int left,right;
	int result;

	/* search for the record */
	while (i <= GetCount(node)
		&& (memcmp(key, GetKey(node,i), keyLength) > 0)) i++;


	if(GetLeaf(node) == LEAF)
	{
		/* is the record in the current node? */
		if (i <= GetCount(node)
			&& (memcmp(key, GetKey(node,i), keyLength) == 0))
		{
			/* just delete the record */
			memmove(GetRecord(node,i),GetRecord(node,i+1),
					(GetCount(node)-i)*recordLength);

			SetCount(node,GetCount(node)-1);
			WriteNode(node);

			result = DELETION_SUCCEEDED;
			goto END;
		}
		else /* the record does not exist in the B-tree */
		{
			result = DELETION_FAILED;
			goto END;
		}
	}
	else /* p->node is an internal node */
	{
		/* is the record in the current (internal) node? */
		if (i <= GetCount(node)
			&& (memcmp(key, GetKey(node,i), keyLength) == 0))
		{
			/* determine the left child node of the searched record */
			y = AllocateNode(node->tree,MODE_MEMORY);
			y->position = GetChild(node,i);
			ReadNode(y);

			/* child y contains at least "minRecords + 1" records */
			if(GetCount(y) >= order)
			{
				/* replace the searched key by its predecessor */
				memcpy(GetRecord(node,i),GetRecord(y,GetCount(y)),recordLength);
				WriteNode(node);

				/* recursively delete the predecessor */
				BtreeDelete(y,GetKey(y,GetCount(y)));
				result = DELETION_SUCCEEDED;
				goto END;
			}
			else /* child y contains exactly "minRecords" records */
			{
				/* determine the right child node of the searched record */
				z = AllocateNode(node->tree,MODE_MEMORY);
				z->position = GetChild(node,i+1);
				ReadNode(z);

				/* child z contains at least "minRecords + 1" records */
				if(GetCount(z) >= order)
				{
					/* replace the searched key by its successor */
					memcpy(GetRecord(node,i), GetRecord(z,1), recordLength);
					WriteNode(node);

					/* recursively delete the successor */
					BtreeDelete(z,key);
					result = DELETION_SUCCEEDED;
					goto END;
				}
				else /* both children y and z contain "minRecords" records */
				{
					BtreeMergeNodes(node,y,i,z);
					/* WriteNode(node) already done by BtreeMergeNodes() */
					BtreeDelete(y,key);
					result = DELETION_SUCCEEDED;
					goto END;

				} /* END OF "if(GetCount(z) >= order)" */

			} /* END OF "if(GetCount(y) >= order)" */
		}
		else /* the record is not in the current (internal) node */
		{
			/* load the node that could contain the searched record */
			c = AllocateNode(node->tree,MODE_MEMORY);
			c->position = GetChild(node,i);
			ReadNode(c);

			if(GetCount(c) == minRecords)
			{
				/* load the siblings of the child node c, if possible */
				left = right = 0;

				if(i > 1)
				{
					y = AllocateNode(node->tree,MODE_MEMORY);
					y->position = GetChild(node,i-1);
					ReadNode(y);

					if(GetCount(y) >= order)
					{
						BtreeReduceSibling(node,c,y,i,TYPE_LEFT);
						left = 1;
					}
					else
					{
						left = -1;
					}
				}

				if(i < GetCount(c) && left < 1)
				{
					z = AllocateNode(node->tree,MODE_MEMORY);
					z->position = GetChild(node,i+1);
					ReadNode(z);

					if(GetCount(z) >= order)
					{
						BtreeReduceSibling(node,c,z,i,TYPE_RIGHT);
						right = 1;
					}
					else
					{
						right = -1;
					}
				}

				/* if both siblings have "minRecords" records */
				if(left == -1)
				{
					/* merge with the left sibling */
					BtreeMergeNodes(node,y,i,c);
					c = 0;
				}
				else if(right == -1)
				{
					/* merge with the right sibling */
					BtreeMergeNodes(node,c,i,z);
					z = 0;
					right = 0;
				}

				if(left != 0 && c != 0)
				{
					free(y->data);
					free(y);
				}

				if(right != 0)
				{
					free(z->data);
					free(z);
				}
			}

			if(c == 0)
				BtreeDelete(y,key);
			else
				BtreeDelete(c,key);

			result = DELETION_SUCCEEDED;
			goto END;

		} /* END OF "the searched record is in an internal node" */

	} /* END OF "if(GetLeaf(node) == LEAF)" */

END:
	if(node->position != node->tree->meta->rootPosition)
	{
		free(node->data);
		free(node);
	}
	return result;
}
