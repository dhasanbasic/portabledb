/*
 * demo.c
 *
 *  Created on: Jul 1, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */

#include <stdio.h>
#include <string.h>

#include "portabledb.h"

void createDemoDatabase();
void describeDemoDatabase();

int main(int argc, char *argv[])
{
	printf("\n------------------------\n");
	printf("  PortableDB Version 0.8  \n");
	printf("\n------------------------\n");

	if (strcmp("create", argv[1]) == 0)
	{
		createDemoDatabase();
	}
	else if (strcmp("describe", argv[1]) == 0)
	{
		describeDemoDatabase();
	}
	else
	{
		printf("\nPlease provide one argument: 'create' or 'describe'\n");
	}
	printf("\n------------------------\n");
	return 0;
}

void createDemoDatabase()
{
	Database *db;
	Table *t;
	FieldType *types = (FieldType *)calloc(2, sizeof(FieldType));

	printf("\nCreating demo database...");

	memcpy(&types[0].name, "INT", 3);
	types[0].length = sizeof(int);

	memcpy(&types[1].name, "CHAR", 4);
	types[1].length = 0;

	CreateDatabase("portable.db", types, 2);
	free(types);

	printf("done.\n");
	printf("Creating demo table...\n\n");

	db = OpenDatabase("portable.db");

	t = DefineTable(db, "Articles", 8, 3);

	DefineField(t, 0, "Id", 2, &db->types[0], 0);
	DefineField(t, 1, "Name", 4, &db->types[1], 20);
	DefineField(t, 2, "Price", 6, &db->types[0], 0);

	SetPrimaryKey(t, 0);

	printf("-> %s\n", t->fields[0].name);
	printf("-> %s\n", t->fields[1].name);
	printf("-> %s\n", t->fields[2].name);

	AddTable(db, t);

	printf("\ndone.\n");

	CloseDatabase(db);
}

void describeDemoDatabase()
{
	Database *db;
	Table *t;

	printf("\nLoading demo database...");

	db = OpenDatabase("portable.db");

	printf("done.\n");

	t = LoadTable(db, 0);

	printf("\nTable: name='%s' id='%d' numFields='%d'\n\n", t->meta->name, t->meta->id, t->meta->numFields);
	printf("-> %s\n", t->fields[0].name);
	printf("-> %s\n", t->fields[1].name);
	printf("-> %s\n", t->fields[2].name);

	CloseDatabase(db);
}
