
#include "btreelayer.h"

#include <string.h>
#include <stdio.h>

char* getRecordPtr(NODE* node, SHORT n) {
	return node->data + RECORDS(node) + n * RECSIZE(node);
}

char* getKeyPtr(NODE* node, SHORT n) {
	return node->data + RECORDS(node) + n * RECSIZE(node) + KEYPOS(node);
}

LONG* getChildPtr(NODE* node, SHORT n) {
	return (LONG*)(node->data + CHILDREN(node) + n * sizeof(LONG));
}

SHORT getCount(NODE* node) {
	return *((SHORT*)(node->data + COUNT(node)));
}

void setCount(NODE* node, SHORT value) {
	*((SHORT*)(node->data + COUNT(node))) = value;
}

SHORT getLeaf(NODE* node) {
	return *((SHORT*)(node->data + LEAF(node)));
}

void setLeaf(NODE* node, SHORT value) {
	*((SHORT*)(node->data + LEAF(node))) = value;
}

void readNode(FILE* file, LONG position, NODE* node)
{
	fseek(file, position, SEEK_SET);
	fread(node->data, NODESIZE(node), 1, file);
}

LONG writeNode(FILE* file, LONG position, NODE* node)
{
	if(position == 0)
	{
		fseek(file, 0, SEEK_END);
		position = ftell(file);
	}
	else
		fseek(file, position, SEEK_SET);
	
	fwrite(node->data, NODESIZE(node), 1, file);
	return position;
}
