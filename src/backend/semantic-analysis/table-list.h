#ifndef CHARLIST_H
#define CHARLIST_H

#include <stdbool.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

typedef struct CharList {
    Node *head;
} CharList;

CharList *initCharList();
bool listFind(CharList *list, const char *data);
void listInsert(CharList *list, const char *data);
void listDelete(CharList *list, const char *data);
void freeCharList(CharList *list);
void printList(CharList *list);

#endif  // CHARLIST_H