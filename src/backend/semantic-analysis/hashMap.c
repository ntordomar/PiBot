#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../support/memory-manager.h"


#define INITIAL_SIZE 20



static uint64_t hash(HashMap *hm, KeyStruct * key) {
    return hm->prehash(key) % hm->totalSize;
}

// Función de comparación de claves para KeyStruct
static bool keyEquals(KeyStruct key1, KeyStruct key2) {
    return 
           strcmp(key1.columnName, key2.columnName) == 0;
}

// Inicialización del HashMap
HashMap* hashMapInit(uint64_t (*prehash)(KeyStruct* key), bool (*keyEquals)(KeyStruct key1, KeyStruct key2)) {
    HashMap *hm = (HashMap *)mm_malloc(sizeof(HashMap));
    hm->totalSize = INITIAL_SIZE;  // Capacidad inicial arbitraria, puedes ajustar según sea necesario
    hm->usedSize = 0;
    hm->threshold = 0.75;  // Factor de carga arbitrario, puedes ajustar según sea necesario
    hm->prehash = prehash;
    hm->keyEquals = keyEquals;
    hm->lookupTable = (EntryStruct *)mm_malloc(hm->totalSize* sizeof(EntryStruct));
    return hm;
}

static void resize(HashMap *hm) {
    EntryStruct * prevTable = hm->lookupTable;
    hm->lookupTable = mm_calloc(hm->totalSize*2, sizeof(EntryStruct));
    if(hm->lookupTable == NULL){
        hm->lookupTable = prevTable;
        return;
    }
    uint64_t oldSize = hm->totalSize;
    hm->totalSize *= 2;
    hm->usedSize = 0;
    for(uint64_t i = 0; i < oldSize; i++) {
        if(prevTable[i].status == OCCUPIED) {
            hashMapInsert(hm, &(prevTable[i].key), &(prevTable[i].value));
        }
    }
}

// Inserción o actualización en el HashMap
void hashMapInsert(HashMap *hm, KeyStruct *key, ValueStruct *value) {
     bool removed = hashMapRemove(hm, key);
    EntryStruct *newEntry = mm_malloc(sizeof(EntryStruct));

    newEntry->key.columnName = mm_malloc(sizeof(char) * (strlen(key->columnName) + 1));
    strcpy(newEntry->key.columnName, key->columnName);
    if(key->tableName != NULL) {
        newEntry->key.tableName = mm_malloc(sizeof(char) * (strlen(key->tableName) + 1));
        strcpy(newEntry->key.tableName, key->tableName);
    }
    newEntry->value.type = value->type;


    newEntry->status = OCCUPIED;

    uint64_t pos = hash(hm, key);
    bool found = false;
    if(hm->lookupTable[pos].status == OCCUPIED) {
        for(uint64_t i = 1; i < hm->totalSize && !found; i++) {
            uint64_t index = (pos+i) % hm->totalSize;
            if(hm->lookupTable[index].status != OCCUPIED) {
                pos = index;
                found = true;
            }
        }
    }    
    if(!removed) {
        hm->usedSize++;
    }
    hm->lookupTable[pos] = *newEntry;
    bool mustResize = ((double)hm->usedSize/hm->totalSize) > hm->threshold;
    if(mustResize) {
        resize(hm);
    }

}




// Eliminación de un elemento en el HashMap
bool hashMapRemove(HashMap *hm, KeyStruct * key) {

    uint64_t firstPosition = hash(hm, key);
    for(uint64_t i = 0; i < hm->totalSize; i++) {
        uint64_t index = (firstPosition + i) % hm->totalSize;
        EntryStruct * aux = &hm->lookupTable[index];
        if(aux->status == EMPTY) {
            return false;
        }
        if(aux->status == OCCUPIED && keyEquals(aux->key, *key)) {
            if(hm->lookupTable[(index+1) % hm->totalSize].status == EMPTY) {
                aux->status = EMPTY;
                hm->totalSize--;
            } else {
                aux->status = DELETED;
            }
            hm->usedSize--;
            return true;
        }
    }
    return false;
}

// Búsqueda de un elemento en el HashMap
EntryStruct *hashMapFind(HashMap *hm, KeyStruct *key, ValueStruct * value) {
    uint64_t firstPosition = hash(hm, key);
    for(uint64_t i = 0; i < hm->totalSize; i++) {
        uint64_t index = (firstPosition + i) % hm->totalSize;
        EntryStruct * aux = &hm->lookupTable[index];
        if(aux->status == EMPTY) return NULL;
        if(aux->status == OCCUPIED && keyEquals(aux->key, *key)) {
            if(value != NULL) {
                value->type = aux->value.type;
            }
            return aux;
        }
    }
    return NULL;
}

// Obtención del tamaño del HashMap
uint64_t hashMapSize(HashMap *hm) {
    return hm->usedSize;
}

