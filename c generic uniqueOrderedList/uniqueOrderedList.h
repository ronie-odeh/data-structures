#ifndef UNIQUEORDEREDLIST_H_
#define UNIQUEORDEREDLIST_H_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
typedef void* Element;

typedef Element (*copyElements)(Element);

typedef void (*freeElements)(Element);

typedef bool (*elementsEquals)(Element, Element);

typedef bool (*elementGreaterThan)(Element e1, Element e2);

typedef struct node_t* Node;

Node nodeCreate(Element data,copyElements copyElements1);
Node nodeCopy(Node node, copyElements copyElements1);

typedef struct uniqueOrderedList_t* UniqueOrderedList;
typedef enum UniqueOrderedListResult_t {
    UNIQUE_ORDERED_LIST_SUCCESS,
    UNIQUE_ORDERED_LIST_OUT_OF_MEMORY,
    UNIQUE_ORDERED_LIST_NULL_ARGUMENT,
    UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST,
    UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS
} UniqueOrderedListResult;
UniqueOrderedList uniqueOrderedListCreate(copyElements, freeElements, elementsEquals, elementGreaterThan);
void uniqueOrderedListDestroy(UniqueOrderedList uniqueOrderedList);
int uniqueOrderedListSize(UniqueOrderedList uniqueOrderedList);
bool uniqueOrderedListContains(UniqueOrderedList uniqueOrderedList,Element element);
UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList uniqueOrderedList, Element element);
Element uniqueOrderedListGetLowest(UniqueOrderedList uniqueOrderedList);
Element uniqueOrderedListGetGreatest(UniqueOrderedList uniqueOrderedList);
Element uniqueOrderedListGetNext(UniqueOrderedList uniqueOrderedList);
void uniqueOrderedListClear(UniqueOrderedList uniqueOrderedList);
UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList uniqueOrderedList, Element element);
UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList uniqueOrderedList);
#endif
