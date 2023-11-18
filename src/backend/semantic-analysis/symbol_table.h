#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "hashmap.h"
#include "table-list.h"



void symbolTableInit();

EntryStruct* symbolTableFind(KeyStruct * key, ValueStruct * value);

void symbolTableInsert(KeyStruct * key, ValueStruct * value);

void symbolTablePrint(); //just for debugging


void symbolTableInsertTable(char * tableName);
bool symbolTableFindTable(char * tableName);
void symbolTableDeleteTable(char * tableName);
void printTableList();

#endif // SYMBOL_TABLE_H