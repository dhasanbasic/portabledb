/*
 * database.h
 *
 *  Created on: Jul 8, 2008
 *      Author: dinko
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "database/dbtypes.h"
#include "btree.h"

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

Table* StartTableDefintion(const short numFields);

void   EndTableDefintion(Table* t);

/* ************************************************************************* */

#endif /* DATABASE_H_ */
