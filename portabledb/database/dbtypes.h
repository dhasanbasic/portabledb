/*
 * dbtypes.h
 *
 *  Created on: Jul 8, 2008
 *      Author: dinko
 */

#ifndef DBTYPES_H_
#define DBTYPES_H_

#include "../btree/bttypes.h"

typedef struct
{
	char	name[8];
	SHORT	length;

} FieldType;

typedef struct
{
	SHORT 	id;
	char	name[32];
	char	primaryKey;
	char	type[8];
	SHORT	length;

} Field;

typedef struct
{
	unsigned char	id;
	char			name[32];
	LONG			dataTree;
	SHORT			numFields;

} TableMeta;

typedef struct
{
	TableMeta*	meta;
	Field**		fields;
	BtTree*		data;

} Table;

typedef struct
{
	SHORT 	numTypes;
	SHORT	numTables;
	LONG	typeList;
	LONG	fieldTree;
	LONG	tableTree;

} DbMeta;

typedef struct
{
	DbMeta*		meta;
	FieldType**	fieldTypes;
	BtTree*		tables;
	BtTree*		fieldTree;

} Database;

#endif /* DBTYPES_H_ */
