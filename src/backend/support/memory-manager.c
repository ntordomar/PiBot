#include "memory-manager.h"
#include "logger.h"

void * mm_malloc(size_t size){
    void * ptr = malloc(size);
    if(ptr == NULL){
        LogError("Error: malloc failed\n");
        mm_freeAll();
        exit(1);
    }

    list * node = malloc(sizeof(list));
    if(node == NULL){
        LogError("Error: malloc failed\n");
        mm_freeAll();
        exit(1);
    }

    node->data = ptr;
    node->next = NULL;

    list * aux = head;
    if(head == NULL){
        head = node;
        return ptr;
    }
    while(aux->next != NULL){
        aux = aux->next;
    }
    aux->next = node;
    return ptr;
}

void * mm_calloc(size_t count ,size_t size){
       void * ptr = calloc(count,size);
    if(ptr == NULL){
        LogError("Error: malloc failed\n");
        mm_freeAll();
        exit(1);
    }

    list * node = malloc(sizeof(list));
    if(node == NULL){
        LogError("Error: malloc failed\n");
        mm_freeAll();
        exit(1);
    }

    node->data = ptr;
    node->next = NULL;

    if(head == NULL){
        head = node;
        return ptr;
    }

    list * aux = head;
    while(aux->next != NULL){
        aux = aux->next;
    }
    aux->next = node;
    return ptr;

}


void mm_freeAll(){
    list * aux = head;
    while(aux != NULL){
        list * aux2 = aux;
        aux = aux->next;
        free(aux2->data);
        free(aux2);
    }
}