#ifndef NODE_H_
#define NODE_H_

#include "types.h"

#include <stdio.h>

NODE* allocateNode(BTREE* btree);

void freeNode(NODE* node);

NODE* loadNode(BTREE* btree, FILE* file, LONG position);

#endif /*NODE_H_*/
