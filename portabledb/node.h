#ifndef NODE_H_
#define NODE_H_

#include "types.h"

NODE* allocateNode(BTREE* btree);

void freeNode(NODE* node);

#endif /*NODE_H_*/
