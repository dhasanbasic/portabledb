/*
 * test.c
 *
 *  Created on: Jul 1, 2008
 *      Author: dinko
 */

#include <stdio.h>
#include <malloc.h>

#include <string.h>

#include "portabledb.h"

int main(void)
{
	Database* db;
//	FieldType* types;

	printf("PortableDB Version 0.8\n------------------------\n\n");
/*
	types = (FieldType*)calloc(2,sizeof(FieldType));
	memcpy(&types[0].name,"INT",3);
	types[0].length = sizeof(int);

	memcpy(&types[1].name,"CHAR",4);
	types[1].length = 0;

	CreateDatabase("portable.db",types,2);

	cfree(types);
	return 0;
*/
	db = OpenDatabase("portable.db");


	CloseDatabase(db);

	return 0;
}

// void main(void) { PrepareNewDb(); }
