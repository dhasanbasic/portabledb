/*
 * dbtypes.h
 *
 *  Created on: Jul 8, 2008
 *      Author: dinko
 */

#ifndef DBTYPES_H_
#define DBTYPES_H_

#include "../btree/bttypes.h"

#include <stdio.h>

typedef struct
{
	char	name[6];
	SHORT	length;

} FieldType;

typedef struct
{
	SHORT 	id;
	char	name[32];
	char	primaryKey;
	char	type[6];
	SHORT	length;

} Field;

typedef struct
{
	SHORT	id;
	char	name[32];
	LONG	dataTree;
	SHORT	numFields;

} TableMeta;

typedef struct
{
	TableMeta*	meta;
	Field*		fields;
	BtTree*		data;

} Table;

typedef struct
{
	SHORT 	numTypes;
	SHORT	numTables;
	LONG	typeList;
	LONG	fieldTree;
	LONG	tableTree;

} DatabaseMeta;

typedef struct
{
	DatabaseMeta*	meta;
	FieldType*		types;
	BtTree*			tables;
	BtTree*			fields;
	FILE*			file;

} Database;

#endif /* DBTYPES_H_ */
