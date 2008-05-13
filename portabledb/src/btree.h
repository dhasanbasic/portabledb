#ifndef BTREE_H_
#define BTREE_H_

#include "types.h"

BTREE* createBtree();

void freeBtree(BTREE* btree);

void loadTree(BTREE* btree);

void saveTree(BTREE* btree);

int recursiveSearch(BPARAM* p);

void insertRecord(BPARAM* p);

#endif /*BTREE_H_*/
