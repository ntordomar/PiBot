#include <stdbool.h>
#include "hashMap.h"
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "abstract-syntax-tree.h"
#include "../support/shared.h"

void checkTableExists(Constant * constant);
void checkValidTableReferences(Columns * columns);
void checkValidTableReference(Column *column);
void checkValidTablesInProgram(Program * program);
bool checkContainsTable(char * table, Columns * sLista);
bool checkListInsideOther(Columns* pLista, Columns * segundaLista);
void checkValidGroupByClause(Program * program);
void checkColumnsType(Constant * leftConstant, Constant * rightConstant);
void createSymbolEntry(KeyStruct * key, ValueStruct * val, char * tableName, char * columnName, TypeColumn type);
void printErrors();