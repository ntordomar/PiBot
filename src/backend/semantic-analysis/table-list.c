#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table-list.h"
#include "../support/logger.h"
#include "../support/memory-manager.h"

CharList *initCharList() {
    CharList *list = (CharList *)mm_malloc(sizeof(CharList));
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
    Node *newNode = (Node *)mm_malloc(sizeof(Node));
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
            return;
        }

        prev = current;
        current = current->next;
    }
}

void freeCharList(CharList *list) {
}


void printList(CharList *list) {
    Node *current = list->head;
    while (current != NULL) {
        current = current->next;
    }
}
