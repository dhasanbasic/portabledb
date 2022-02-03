/*
 * database.c
 *
 *  Created on: Jul 8, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */

#include "../portabledb.h"

#include <stdio.h>

/* Creates an empty database */

void CreateDatabase(
		const char* 		filename,
		const FieldType* 	types,
		const SHORT 		numTypes)
{
	Database* db = (Database*)malloc(sizeof(Database));

	/* create an empty binary-data file */
	remove(filename);
	db->file = fopen(filename,"w+b");
	db->file = freopen(filename,"r+b",db->file);

	/* initialize the database structures */
	db->meta = (DatabaseMeta*)malloc(sizeof(DatabaseMeta));
	db->meta->numTables = 0;
	db->meta->numTypes = numTypes;
	db->meta->typeList = sizeof(DatabaseMeta);

	/* first pass (without the B-trees) */
	fwrite(db->meta,sizeof(DatabaseMeta),1,db->file);
	fwrite(types,sizeof(FieldType),numTypes,db->file);

	/* now create the B-trees */
	db->tables = CreateTree(db->file,10,sizeof(TableMeta),0,sizeof(SHORT),8);
	db->fields = CreateTree(db->file,10,sizeof(TableMeta),0,sizeof(SHORT),8);

	/* save the positions of the B-trees */
	db->meta->tableTree = db->tables->position;
	db->meta->fieldTree = db->fields->position;

	/* second pass */
	fseek(db->file,0,SEEK_SET);
	fwrite(db->meta,sizeof(DatabaseMeta),1,db->file);
	fclose(db->file);

	/* free allocated resources */
	FreeTree(db->tables);
	FreeTree(db->fields);
	free(db->meta);
	free(db);
}

Database* OpenDatabase(const char* filename)
{
	Database* db = (Database*)malloc(sizeof(Database));

	/* read the meta-data */
	db->meta = (DatabaseMeta*)malloc(sizeof(DatabaseMeta));
	db->file = fopen(filename,"r+b");
	fread(db->meta,sizeof(DatabaseMeta),1,db->file);

	/* load the field types */
	db->types = (FieldType*)calloc(db->meta->numTypes,sizeof(FieldType));
	fseek(db->file,db->meta->typeList,SEEK_SET);
	fread(db->types,sizeof(FieldType),db->meta->numTypes,db->file);

	/* read the B-trees */
	db->tables = ReadTree(db->file, db->meta->tableTree);
	db->fields = ReadTree(db->file, db->meta->fieldTree);

	return db;
}

void CloseDatabase(Database* db)
{
	/* write the B-trees */
	WriteTree(db->tables);
	WriteTree(db->fields);

	/* write the table meta-data */
	fseek(db->file,0,SEEK_SET);
	fwrite(db->meta,sizeof(DatabaseMeta),1,db->file);

	/* free any used space */
	FreeTree(db->tables);
	FreeTree(db->fields);
	free(db->meta);
	free(db->types);
	fclose(db->file);
	free(db);
}
