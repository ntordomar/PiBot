#include <stdio.h>
#include <string.h>
#include "symbol_table.h"


#define SEED 0x13572468

static CharList *tableList = NULL;
static HashMap *symbol_table = NULL;

// MurmurOAAT_32_Modified
static uint64_t symbolHashFunction(KeyStruct *key) {

    uint64_t h = SEED;


    const char * str = key->tableName;
    const char * str2 = key->columnName;
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    if(str != NULL){
        for (; *str; ++str) {
            h ^= *str;
            h *= 0x5bd1e995;
            h ^= h >> 15;
        }
    }

    if(str2 != NULL){
        for(;*str2; ++str2) {
            h ^= *str2;
            h *= 0x5bd1e995;
            h ^= h >> 15;
        }
    }
    return h;
}

static bool symbolKeyEquals(KeyStruct key1, KeyStruct key2) {
    if(key1.tableName != NULL && key2.tableName != NULL) {
        return strcmp(key1.tableName, key2.tableName) == 0 && strcmp(key1.columnName, key2.columnName) == 0;
    }
    if(key1.tableName != NULL || key2.tableName != NULL) {
        return false;
    } 
    return strcmp(key1.columnName, key2.columnName) == 0;
}


void symbolTableInit() {
    if(symbol_table != NULL) {
        return;
    }
    if(tableList != NULL) {
        return;
    }
    symbol_table = hashMapInit(symbolHashFunction, symbolKeyEquals);
    tableList = initCharList();

    
}

EntryStruct *symbolTableFind(KeyStruct * key, ValueStruct * value){
    if(symbol_table == NULL){
        return NULL;
    }
    return hashMapFind(symbol_table, key, value);
}

void symbolTableInsert(KeyStruct * key, ValueStruct * value){
    if(symbol_table == NULL){
        return;
    }
    hashMapInsert(symbol_table, key, value);
}

void symbolTablePrint(){
    for(int i = 0; i< symbol_table->totalSize; i++){
        if(symbol_table->lookupTable[i].status == OCCUPIED){
            printf("key: %s , value: %d\n", symbol_table->lookupTable[i].key.columnName, symbol_table->lookupTable[i].value.type);
        }
    }
}

void symbolTableInsertTable(char * tableName){
    listInsert(tableList, tableName);
}

bool symbolTableFindTable(char * tableName){
    return listFind(tableList, tableName);
}



void symbolTableDeleteTable(char * tableName){
    listDelete(tableList,tableName);
}

void printTableList(){
    printList(tableList);
}


