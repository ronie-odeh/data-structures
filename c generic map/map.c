#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


/*
 * struct node to contain the element and the key, node* next,prev to move to
 * the next node or the previous node.
 */
typedef struct node{
    MapDataElement dataElement;
    MapKeyElement keyElement;
    struct node* next;
    struct node* prev;
}*Node;

/*
 * struct map contain the functions to free/copy the data and the key and
 * compare the keys, the size of the map and node list of the elements of
 * the map.
 */
struct Map_t {
    int size;
    Node head;
    Node tail;
    Node iterator;
    copyMapDataElements copyDataElement_map;
    copyMapKeyElements copyKeyElement_map;
    freeMapDataElements freeDataElement_map;
    freeMapKeyElements freeKeyElement_map;
    compareMapKeyElements compareKeyElements_map;
};

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){



    if(!compareKeyElements || !freeDataElement || !freeKeyElement
       || !copyKeyElement || !copyDataElement){
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    if(!map){
        return NULL;
    }

    map->size = 0;
    map->compareKeyElements_map = compareKeyElements;
    map->copyDataElement_map = copyDataElement;
    map->copyKeyElement_map = copyKeyElement;
    map->freeDataElement_map = freeDataElement;
    map->freeKeyElement_map = freeKeyElement;

    map->head = malloc(sizeof(*map->head));
    if(!map->head){
        free(map);
        return NULL;
    }
    map->tail = malloc(sizeof(*map->tail));
    if(!map->tail){
        free(map->head);
        free(map);
        return NULL;
    }
    map->head->next = map->tail;
    map->head->prev = NULL;
    map->head->keyElement = NULL;
    map->head->dataElement = NULL;
    map->tail->next = NULL;
    map->tail->prev = map->head;
    map->tail->keyElement = NULL;
    map->tail->dataElement = NULL;
    map->iterator = NULL;

    return map;
}

int mapGetSize(Map map){
    if(!map){
        return -1;
    }
    return map->size;
}


MapResult mapClear(Map map){
    if(!map){
        return MAP_NULL_ARGUMENT;
    }
    Node current;
    Node to_delete = map->head->next;
    while(to_delete != map->tail){
        current = to_delete->next;
        map->freeKeyElement_map(to_delete -> keyElement);
        map->freeDataElement_map(to_delete -> dataElement);
        free(to_delete);
        to_delete = current;
    }
    map->size = 0;
    map->head->next = map->tail;
    map->tail->prev = map->head;

    return MAP_SUCCESS;

}

void mapDestroy(Map map){
    if(!map){
        return;
    }
    mapClear(map);
    free(map->head);
    free(map->tail);
    free(map);
}

MapKeyElement mapGetFirst(Map map){
    if(!map){
        return NULL;
    }

    if(map->size == 0){
        return NULL;
    }
    map->iterator = map->head->next;
    return map->iterator->keyElement;
}


MapKeyElement mapGetNext(Map map){
    if(!map){
        return NULL;
    }

    if(map->iterator == NULL){
        return NULL;
    }

    if(map->iterator->next == map->tail){
        return NULL;
    }

    map->iterator = map->iterator->next;
    return map->iterator->keyElement;
}

bool mapContains(Map map, MapKeyElement element){
    if(!map || !element){
        return false;
    }
    MAP_FOREACH(MapKeyElement,iter,map){
        if(map->compareKeyElements_map(iter,element) == 0){
            return true;
        }
    }
    return false;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if(!map || !keyElement){
        return NULL;
    }
    MAP_FOREACH(MapKeyElement,iter,map){
        if(map->compareKeyElements_map(iter,keyElement) == 0){
            return map->iterator->dataElement;
        }
    }
    return NULL;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if(!map || !keyElement || !dataElement){
        return MAP_NULL_ARGUMENT;
    }

    Node new_node = malloc(sizeof(*new_node));
    if(!new_node){
        return MAP_OUT_OF_MEMORY;
    }

    new_node->keyElement = map->copyKeyElement_map(keyElement);
    if(!new_node->keyElement){
        free(new_node);
        return MAP_OUT_OF_MEMORY;
    }
    new_node->dataElement = map->copyDataElement_map(dataElement);
    if(!new_node->dataElement){
        free(new_node->keyElement);
        free(new_node);
        return MAP_OUT_OF_MEMORY;
    }

    if(mapContains(map,keyElement)){
        MapResult result = mapRemove(map,keyElement);
        assert(result == MAP_SUCCESS);
    }

    if(map->size == 0){
        new_node->next = map->tail;
        new_node->prev = map->head;
        map->head->next = new_node;
        map->tail->prev = new_node;
        map->size++;
        return MAP_SUCCESS;
    }

    MAP_FOREACH(MapKeyElement ,iter,map){
        if(map->compareKeyElements_map(keyElement,iter) < 0 ){
            new_node->next = map->iterator;
            new_node->prev = map->iterator->prev;

            new_node->prev->next = new_node;
            map->iterator->prev = new_node;
            map->size++;
            return MAP_SUCCESS;
        }
    }
    assert(map->iterator == NULL);
    new_node->next = map->tail;
    new_node->prev = map->tail->prev;
    new_node->prev->next = new_node;
    map->tail->prev = new_node;
    map->size++;
    return MAP_SUCCESS;
}

MapResult mapRemove(Map map, MapKeyElement keyElement){
    if(!map || !keyElement){
        return MAP_NULL_ARGUMENT;
    }

    if(!mapContains(map,keyElement)){
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    MAP_FOREACH(MapKeyElement,iter,map){
        if(map->compareKeyElements_map(iter,keyElement) == 0){
            Node prev_to_iterator,next_to_iterator;
            prev_to_iterator = map->iterator->prev;
            next_to_iterator = map->iterator->next;

            prev_to_iterator->next = next_to_iterator;
            next_to_iterator->prev = prev_to_iterator;

            map->freeDataElement_map(map->iterator->dataElement);
            map->freeKeyElement_map(map->iterator->keyElement);

            free(map->iterator);
            map->iterator = NULL;
            map->size--;
            return MAP_SUCCESS;
        }
    }
    //shouldnt get here
    return MAP_NULL_ARGUMENT;
}

Map mapCopy(Map map){
    if(!map){
        return NULL;
    }

    Map new_map = mapCreate(map->copyDataElement_map,map->copyKeyElement_map,
                            map->freeDataElement_map,map->freeKeyElement_map,
                            map->compareKeyElements_map);
    if(!new_map){
        return NULL;
    }


    MAP_FOREACH(MapKeyElement,iter,map){
        assert(map->iterator != NULL);
        if(mapPut(new_map,map->iterator->keyElement,map->iterator->dataElement)
           == MAP_OUT_OF_MEMORY){
            mapDestroy(new_map);
            return NULL;
        }
    }
    return new_map;
}