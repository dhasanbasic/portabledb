#ifndef BTREE_H_
#define BTREE_H_

#include "types.h"

BTREE* allocateBtree(INT order, INT recordSize, INT key, INT keySize);

void freeBtree(BTREE* btree);

#endif /*BTREE_H_*/
