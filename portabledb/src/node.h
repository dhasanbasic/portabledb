#ifndef NODE_H_
#define NODE_H_

#include "types.h"

NODE* createNode(BTREE* btree);

void freeNode(NODE* node);

void loadNode(NODE* node);

void saveNode(NODE* node);

#endif /*NODE_H_*/
