
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "node.h"

int main(void)
{
    BTREE* btree = malloc(sizeof(BTREE));
    NODE* node = 0;

    int i1 = 3;
    int i2 = 2300;
    int i3 = 323;
    int i4 = 5555;
    int var;

    btree->order = 3;
    btree->key = 0;
    btree->keySize = 4;
    btree->recordSize = 4;
    btree->root = 0;

    node = allocateNode(btree);

    memcpy(node->data, &i1, sizeof(int));
    memcpy(node->data+4, &i2, sizeof(int));
    memcpy(node->data+8, &i3, sizeof(int));
    memcpy(node->data+12, &i4, sizeof(int));
    memcpy(node->data+20, &i1, sizeof(int));

    memcpy(&var, node->data, sizeof(int));
    printf("%d\n", var);

    memcpy(&var, node->data+4, sizeof(int));
    printf("%d\n", var);

    memcpy(&var, node->data+8, sizeof(int));
    printf("%d\n", var);

    memcpy(&var, node->data+12, sizeof(int));
    printf("%d\n", var);

    memcpy(&var, node->data+20, sizeof(int));
    printf("%d\n", var);

    return 0;
}
