/*
 * database.h
 *
 *  Created on: Jul 8, 2008
 *      Author: Dinko Hasanbasic (dinko.hasanbasic@gmail.com)
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "btree.h"
#include "database/dbtypes.h"

#include <stdio.h>

/* ************************************************************************* */
/*                            Database operations                            */
/* ************************************************************************* */

void CreateDatabase(
		const char* 		filename,
		const FieldType* 	types,
		const SHORT 		numTypes);

Database* OpenDatabase(const char* filename);

void CloseDatabase(Database* db);

/* ************************************************************************* */

/* ************************************************************************* */
/*                               Table operations                            */
/* ************************************************************************* */

Table* DefineTable(
		const Database* db,
		const char* name,
		const SHORT nameLength,
		const SHORT numFields);

void DefineField(
		Table*				t,
		const SHORT			index,
		const char*			name,
		const SHORT			nameLength,
		const FieldType*	type,
		const SHORT			length);

void SetPrimaryKey(Table* t, const SHORT index);

void AddTable(Database* db,Table* t);

Table* LoadTable(const Database* db, const SHORT id);

/* ************************************************************************* */

#endif /* DATABASE_H_ */
