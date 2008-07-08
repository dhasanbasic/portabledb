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
	Table* t;
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
/*
	t = DefineTable(db,"Artikli",10,3);

		DefineField(t,0,"Sifra",5,&db->types[0],0);
		DefineField(t,1,"Naziv",5,&db->types[1],20);
		DefineField(t,2,"Cijena",6,&db->types[0],0);

		SetPrimaryKey(t,0);

	AddTable(db,t);
*/
	t = LoadTable(db,0);

	printf("%s\n",t->fields[0].name);
	printf("%s\n",t->fields[1].name);
	printf("%s\n",t->fields[2].name);

	CloseDatabase(db);

	return 0;
}

// void main(void) { PrepareNewDb(); }
