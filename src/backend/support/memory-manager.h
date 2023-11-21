#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct _list{
    void * data;
    struct _list * next;
} list;

static list * head = NULL;
void * mm_malloc(size_t size);
void mm_freeAll();
void * mm_calloc(size_t count ,size_t size);



#endif // MEMORY_MANAGER_H