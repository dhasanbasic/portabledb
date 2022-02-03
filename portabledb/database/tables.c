/*
 * tables.c
 *
 *  Created on: Jul 8, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */

#include "../portabledb.h"

#include <stdio.h>
#include <string.h>

Table* DefineTable(
		const Database* db,
		const char* name,
		const SHORT nameLength,
		const SHORT numFields)
{
	Table* t = (Table*)malloc(sizeof(Table));

	t->meta = (TableMeta*)malloc(sizeof(TableMeta));

	t->meta->id = db->meta->numTables;
	t->meta->numFields = numFields;
	strncpy(t->meta->name,name,nameLength);

	t->fields = (Field*)calloc(numFields,sizeof(Field));

	return t;
}

void DefineField(
		Table*				t,
		const SHORT			index,
		const char*			name,
		const SHORT			nameLength,
		const FieldType*	type,
		const SHORT			length)
{
	Field* f = &t->fields[index];

	strncpy(f->name,name,nameLength);
	strncpy(f->type,type->name,6);
	f->length = length;
	f->primaryKey = 0;
	f->id = t->meta->id*256 + index;
}

void SetPrimaryKey(Table* t, const SHORT index)
{
	t->fields[index].primaryKey = 1;
}

void AddTable(
		Database* db,
		Table* t)
{
	SHORT i = 0, j = 0;
	SHORT recordLength = 0,length,order,keyPosition,keyLength;

	/* determine the B-tree for the fields */
	while(i < t->meta->numFields)
	{
		if(t->fields[i].length == 0)
		{
			/* use the length from the type definition */
			j=0;
			while(strncmp(db->types[j].name,t->fields[i].type,6) != 0) j++;
			length = db->types[j].length;
		}
		else
			length = t->fields[i].length;

		if(t->fields[i].primaryKey == 1)
		{
			keyPosition = recordLength;
			keyLength = length;
		}

		/* next field... */
		recordLength += length;
		length = 0;
		i++;
	}

	/* the size of a node should be as possible as close to 1024 */
	order = (((1024-2*sizeof(SHORT)+sizeof(LONG))
			/ (recordLength+sizeof(LONG))) + 1) / 2;

	/* create the B-tree to save its position */
	t->data = CreateTree(db->file,order,recordLength,keyPosition,keyLength,8);
	t->meta->dataTree = t->data->position;
	FreeTree(t->data);

	/* add the new table to the tables B-tree */
	InsertRecord(db->tables,t->meta);
	db->meta->numTables++;

	/* add the field definitions to the field B-tree */
	for(i=0; i < t->meta->numFields; i++)
		InsertRecord(db->fields,&t->fields[i]);

	/* free any unneeded resources */
	free(t->fields);
	free(t->meta);
	free(t);
}

Table* LoadTable(const Database* db, const SHORT id)
{
	Table* t = (Table*)malloc(sizeof(Table));
	t->meta = (TableMeta*)malloc(sizeof(TableMeta));
	SHORT i;
	SHORT key;

	/* retrieve the table meta from the tables B-tree */
	SearchRecord(db->tables,&id,t->meta);

	t->fields = (Field*)calloc(sizeof(Field),t->meta->numFields);
	for(i=0; i < t->meta->numFields; i++)
	{
		key = id*256+i;
		SearchRecord(db->fields,&key,t->fields+i);
	}

	return t;
}
