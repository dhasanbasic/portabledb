#ifndef TYPES_H_
#define TYPES_H_

/* ****************************************************************************
 *      COMMON USED TYPES
 * ****************************************************************************/

typedef unsigned char CHAR;
typedef unsigned int  INT;
typedef unsigned long LONG;

/* ****************************************************************************
 *      THE B-TREE DATA STRUCTURE
 * ****************************************************************************/

typedef struct
{
    INT     order;          /* order of the B-tree                  */
    INT     recordSize;     /* size of a record                     */
    INT     key;            /* position of records' key             */
    INT     keySize;        /* size of a records' key               */
    LONG    root;           /* position of the B-trees' root node   */
}
 BTREE;

/* ****************************************************************************
 *      THE B-TREE NODE DATA STRUCTURE
 * ****************************************************************************/

typedef struct
{
    INT     recordCount;    /* count of records in the node         */
    LONG    subnodeLeft;    /* position of the nodes' left subnode  */
    char*   data;           /* data records in the node             */
    LONG*   subnodes;       /* positions of the remaining subnodes  */
}
 NODE;

#endif /*TYPES_H_*/
