#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "uniqueOrderedList.h"
#define ZERO 0

struct node_t{
   Element data;
   struct node_t* next;
};

Node nodeCopy(Node node,copyElements copyElements1) {
    Node newNode = nodeCreate(node->data, copyElements1);
    if (!newNode)
        return NULL;
    newNode->next = NULL;
    return newNode;
}

struct uniqueOrderedList_t {
    int size;
    Node head;
    Node iterator;
    copyElements copyElements1;
    freeElements freeElements1;
    elementsEquals elementsEquals1;
    elementGreaterThan elementGreaterThan1;
};

Node nodeCreate(Element data,copyElements copyElements1){
    if(!data)
        return NULL;
    Node node=malloc(sizeof(*node));
    if(!node)
        return NULL;
    node->data=copyElements1(data);
    if(!node->data){
        free(node);
        return NULL;
    }
    node->next=NULL;
    return node;
}

UniqueOrderedList uniqueOrderedListCreate(copyElements copy_e, freeElements free_e, elementsEquals equal_e,
        elementGreaterThan greater_e){
    UniqueOrderedList uniqueOrderedList=malloc(sizeof(*uniqueOrderedList));
    if(!uniqueOrderedList)
        return NULL;
    uniqueOrderedList->head=NULL;
    uniqueOrderedList->iterator= NULL;
    uniqueOrderedList->size=ZERO;
    uniqueOrderedList->copyElements1=copy_e;
    uniqueOrderedList->freeElements1=free_e;
    uniqueOrderedList->elementsEquals1=equal_e;
    uniqueOrderedList->elementGreaterThan1=greater_e;
    return uniqueOrderedList;
}

int uniqueOrderedListSize(UniqueOrderedList uniqueOrderedList){
    if(!uniqueOrderedList)
        return ZERO;
    int size=uniqueOrderedList->size;
    return size;
}

Element uniqueOrderedListGetNext(UniqueOrderedList uniqueOrderedList){
    if(!uniqueOrderedList)
        return NULL;
    if(!uniqueOrderedList->iterator)
        return NULL;
    if(uniqueOrderedList->iterator->next){
        uniqueOrderedList->iterator=uniqueOrderedList->iterator->next;
        return uniqueOrderedList->iterator->data;
    }
    uniqueOrderedList->iterator=NULL;
    return NULL;
}

bool uniqueOrderedListContains(UniqueOrderedList uniqueOrderedList,Element element){
    if(!uniqueOrderedList->head || !uniqueOrderedList->head->data)
        return false;
    Element ptr=uniqueOrderedListGetLowest(uniqueOrderedList);
    while(ptr)
    {
        if(uniqueOrderedList->elementsEquals1(ptr,element)) {
            return true;
        }
        ptr=uniqueOrderedListGetNext(uniqueOrderedList);
    }
    return false;
}


UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList uniqueOrderedList, Element element){
    if( !element|| !uniqueOrderedList )
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    if(uniqueOrderedListContains(uniqueOrderedList,element))
        return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
    Node ptr, new_item = nodeCreate(element,uniqueOrderedList->copyElements1);
    if(!new_item)
        return UNIQUE_ORDERED_LIST_OUT_OF_MEMORY;
    ptr = uniqueOrderedList->head;
    if(!ptr)
    {
        uniqueOrderedList->head=new_item;
        uniqueOrderedList->size++;
        uniqueOrderedList->iterator=new_item;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    if(uniqueOrderedList->elementGreaterThan1(uniqueOrderedListGetLowest(uniqueOrderedList),element))
    {
        new_item->next=uniqueOrderedList->head;
        uniqueOrderedList->head=new_item;
        uniqueOrderedList->size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    while((ptr->next)&&uniqueOrderedList->elementGreaterThan1(element,((ptr->next->data))))
    {
        ptr=ptr->next;
        if(!ptr->next)
        {
            ptr->next=new_item;
            uniqueOrderedList->size++;
            return UNIQUE_ORDERED_LIST_SUCCESS;
        }
    }
    new_item->next=ptr->next;
    ptr->next=new_item;
    uniqueOrderedList->size++;
    return UNIQUE_ORDERED_LIST_SUCCESS;
}



Element uniqueOrderedListGetLowest(UniqueOrderedList uniqueOrderedList){
    if(!uniqueOrderedList||!uniqueOrderedList->head)
        return NULL;
    Element min=uniqueOrderedList->head->data;
    uniqueOrderedList->iterator=uniqueOrderedList->head;
    return min;
}

Element uniqueOrderedListGetGreatest(UniqueOrderedList uniqueOrderedList){
    if(!uniqueOrderedList)
        return NULL;
    if(!uniqueOrderedList->head)
        return NULL;
    Element max=uniqueOrderedList->head->data;
    Node ptr = uniqueOrderedList->head;
    while(ptr)
    {
        if(uniqueOrderedList->elementGreaterThan1(ptr->data,max)){
            max=ptr->data;
            uniqueOrderedList->iterator=ptr;
        }
        ptr=ptr->next;
    }
    return max;
}

void uniqueOrderedListDestroy(UniqueOrderedList uniqueOrderedList){
    if(uniqueOrderedList){
    uniqueOrderedListClear(uniqueOrderedList);
    free(uniqueOrderedList);}
}


void uniqueOrderedListClear(UniqueOrderedList uniqueOrderedList){
    Node to_delete;
    while(uniqueOrderedList->head)
    {
        to_delete = uniqueOrderedList->head;
        uniqueOrderedList->head = uniqueOrderedList->head->next;
        uniqueOrderedList->freeElements1(to_delete->data);
        free(to_delete);
    }
    uniqueOrderedList->size=ZERO;
}


UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList uniqueOrderedList, Element element){
    Node ptr= uniqueOrderedList->head, ptr2;
    if (!uniqueOrderedListContains(uniqueOrderedList, element))
        return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
    if(uniqueOrderedList->elementsEquals1(ptr->data,element))
    {
        uniqueOrderedList->head=ptr->next;
        uniqueOrderedList->size--;
        uniqueOrderedList->freeElements1(ptr->data);
        free(ptr);
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    else {
        while(!uniqueOrderedList->elementsEquals1(ptr->next->data,element))
        {
            ptr = ptr->next;
        }
        ptr2=ptr->next->next;
        uniqueOrderedList->freeElements1(ptr->next->data);
        free(ptr->next);
        ptr->next=ptr2;
        uniqueOrderedList->size--;
        return UNIQUE_ORDERED_LIST_SUCCESS;
        }
}

UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList uniqueOrderedList){
    UniqueOrderedList  new_uol = uniqueOrderedListCreate(
            uniqueOrderedList->copyElements1,
            uniqueOrderedList->freeElements1,
            uniqueOrderedList->elementsEquals1,
            uniqueOrderedList->elementGreaterThan1);
    if(!new_uol){
        return NULL;
    }
    Node new_node=uniqueOrderedListGetLowest(uniqueOrderedList);
    //printf("\nkkk %s kkk\n",(char*)new_node);
    while(new_node)
    {
        uniqueOrderedListInsert(new_uol,new_node);
        new_node=uniqueOrderedListGetNext(uniqueOrderedList);
    }
    new_uol->iterator= new_uol->head;
    return new_uol;
}


