
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "node.h"
#include "btree.h"

int main(void)
{
    BTREE* btree = allocateBtree(3,4,0,4);
    NODE* node = 0;

    int i1 = 3;
    int i2 = 2300;
    int i3 = 323;
    int var;

    node = allocateNode(btree);

    memcpy(node->data, &i1, sizeof(int));
    memcpy(node->data+4, &i2, sizeof(int));
    memcpy(node->data+8, &i3, sizeof(int));
    
    memcpy(&var, node->data, sizeof(int));
    printf("%d\n", var);

    memcpy(&var, node->data+4, sizeof(int));
    printf("%d\n", var);

    memcpy(&var, node->data+8, sizeof(int));
    printf("%d\n", var);

    node->subnodes[0] = 1;
    node->subnodes[1] = 2;
    
    freeNode(node);
    freeBtree(btree);
    
    return 0;
}
