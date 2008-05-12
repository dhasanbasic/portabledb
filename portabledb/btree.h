#ifndef BTREE_H_
#define BTREE_H_

#include "types.h"

BTREE* allocateBtree(INT order, INT recordSize, INT key, INT keySize);

void freeBtree(BTREE* btree);

void insertRecord(BTREE* btree, NODE* root, void* record);

#endif /*BTREE_H_*/
