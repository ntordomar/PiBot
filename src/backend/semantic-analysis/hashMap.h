#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdbool.h>


// Enum para los tipos de columnas
typedef enum {
    INT_T = 0,
    STRING_T,
    UNDEFINED
} TypeColumn;

// Struct para la clave
typedef struct {
    char *tableName;
    char *columnName;
} KeyStruct;

// Struct para el valor
typedef struct {
    TypeColumn type;
} ValueStruct;

typedef enum {
    EMPTY,
    OCCUPIED,
    DELETED
} Status;

// Struct para el valor
typedef struct {
    KeyStruct key;
    ValueStruct value;
    Status status;
} EntryStruct;


// Definición del HashMap
typedef struct {
    uint64_t totalSize;
    uint64_t usedSize;
    double threshold;
    uint64_t (*prehash)(KeyStruct * key);
    bool (*keyEquals)(KeyStruct key1, KeyStruct key2);
    EntryStruct *lookupTable;
} HashMap;

// Funciones del HashMap
HashMap* hashMapInit(uint64_t (*prehash)(KeyStruct *key), bool (*keyEquals)(KeyStruct key1, KeyStruct key2));
void hashMapInsert(HashMap *hm, KeyStruct *key, ValueStruct *value);
bool hashMapRemove(HashMap *hm, KeyStruct *key);
EntryStruct* hashMapFind(HashMap *hm, KeyStruct *key, ValueStruct *value);
uint64_t hashMapSize(HashMap *hm);
void hashMapDestroy(HashMap *hm);

#endif // HASHMAP_H
