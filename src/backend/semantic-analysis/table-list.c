#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table-list.h"

CharList *initCharList() {
    CharList *list = (CharList *)malloc(sizeof(CharList));
    if (!list) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la lista de caracteres.\n");
        exit(EXIT_FAILURE);
    }

    list->head = NULL;
    return list;
}

bool listFind(CharList *list, const char *data) {
    Node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->data, data) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

void listInsert(CharList *list, const char *data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo nodo.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = strdup(data);
    newNode->next = list->head;
    list->head = newNode;
}

void listDelete(CharList *list, const char *data) {
    Node *current = list->head;
    Node *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->data, data) == 0) {
            if (prev == NULL) {
                list->head = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->data);
            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }
}

void freeCharList(CharList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        if (temp->data != NULL) free(temp->data);
        free(temp);
    }

    free(list);
}


// haceme una funcion que me imprima la lista

void printList(CharList *list) {
    Node *current = list->head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}
