#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./util.h"
#include "./map.h"
#include "./map_node.h"

#define NULL_MAP_GET_SIZE -1

struct Map_t {
    Map_node head_node;
    int size;
    Map_node iterator_node;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

/**
* iteratorReset: Resets the Map iterator
*
* @param map - Pointer to Map that need to reset it's iterator.
*/
static void iteratorReset(Map map) {
    RETURN_VALUE_IF_TRUE(!map,);
    map -> iterator_node = NULL;
}

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements) 
{
    RETURN_VALUE_IF_TRUE(!copyDataElement || !copyKeyElement || !freeDataElement
                             || !freeKeyElement || !compareKeyElements,NULL);
    Map map = malloc(sizeof(*map));
    RETURN_VALUE_IF_TRUE(!map,NULL);
    map -> head_node = NULL;
    map -> size = 0;
    map -> iterator_node = NULL;
    map -> copyDataElement = copyDataElement;
    map -> copyKeyElement = copyKeyElement;
    map -> freeDataElement = freeDataElement;
    map -> freeKeyElement = freeKeyElement;
    map -> compareKeyElements = compareKeyElements;
    return map;
}

void mapDestroy(Map map) {
    RETURN_VALUE_IF_TRUE(!map,);
    mapClear(map);
    free(map);
}

Map mapCopy(Map map) {
    RETURN_VALUE_IF_TRUE(!map,NULL);
    Map mapCopy = mapCreate(map -> copyDataElement,map -> copyKeyElement,
                  map -> freeDataElement,map -> freeKeyElement,
                  map -> compareKeyElements);
    RETURN_VALUE_IF_TRUE(!mapCopy,NULL);
    MapKeyElement current_key = mapGetFirst(map);
    MapDataElement current_data = mapGet(map,current_key);
    while(current_key) {
        RETURN_VALUE_IF_TRUE(mapPut(mapCopy,current_key,current_data)==MAP_OUT_OF_MEMORY,NULL);
        map->freeKeyElement(current_key);
        current_key = mapGetNext(map);
        current_data = mapGet(map,current_key);
    }
    return mapCopy;
}

int mapGetSize(Map map) {
    if(!map) {
        return NULL_MAP_GET_SIZE;
    }
    return map -> size;
}

bool mapContains(Map map, MapKeyElement element) {
    RETURN_VALUE_IF_TRUE(!map || !element,false);
    Map_node current_node = map -> head_node;
    while (current_node) {
        RETURN_VALUE_IF_TRUE(map -> compareKeyElements(nodeGetKey(current_node),element) == 0,true);
        current_node = nodeGetNext(current_node);
    }
    return false;
}

/**
*	AddNewNode: Adds new node to the map sorted in ascending order
*
*   All arguments are given by mapPut() that checks every argument is not null,
*   therefore this function dosen't check it.
*
* @param map - The map for which to reassign the data element
* @param key_copy - The key element which need to be reassigned
* @param data_copy - The new data element to associate with the given key.
*
* @return
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element or a key failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
*/
static MapResult AddNewNodeToMap(Map map, MapKeyElement key_copy, MapDataElement data_copy, Map_node current_node) {
    Map_node new_node = createNode(data_copy,key_copy);
    if(!new_node) {
        map->freeDataElement(data_copy);
        map->freeKeyElement(key_copy);
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    if(!current_node) {
        map->head_node = new_node;
        map->size++;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    if(map->compareKeyElements(nodeGetKey(current_node),key_copy) > 0) {
        map->head_node = new_node;
        nodeSetNext(new_node,current_node);
        map->size++;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    Map_node next_node = nodeGetNext(current_node);
    while (next_node) {
        if (map->compareKeyElements(nodeGetKey(next_node),key_copy) > 0) {
            nodeSetNext(current_node,new_node);
            nodeSetNext(new_node,next_node);
            map->size++;
            iteratorReset(map);
            return MAP_SUCCESS;
        }
        current_node = next_node;
        next_node = nodeGetNext(current_node);
    }
    nodeSetNext(current_node,new_node);
    map->size++;
    iteratorReset(map);
    return MAP_SUCCESS;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if(!map || !keyElement || !dataElement) {
        iteratorReset(map);
        return MAP_NULL_ARGUMENT;
    }
    Map_node current_node = map -> head_node;
    MapDataElement data_copy= map->copyDataElement(dataElement);
    if(!data_copy) {
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    MapKeyElement key_copy= map->copyKeyElement(keyElement);
    if(!key_copy) {
        map->freeDataElement(data_copy);
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    if(mapContains(map,key_copy)) {
        while (current_node) {
            if (map->compareKeyElements(nodeGetKey(current_node),key_copy) == 0)
            {
                map->freeDataElement(nodeGetData(current_node));
                map->freeKeyElement(key_copy);
                nodeSetData(current_node,data_copy);
                iteratorReset(map);
                return MAP_SUCCESS;
            }
            current_node= nodeGetNext(current_node);
        }
    }
    return AddNewNodeToMap(map,key_copy,data_copy,current_node);
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    RETURN_VALUE_IF_TRUE(!map || !keyElement,NULL);
    Map_node current_node = map -> head_node;
    while (current_node)
    {
        RETURN_VALUE_IF_TRUE(map -> compareKeyElements(nodeGetKey(current_node),keyElement) == 0,
                                nodeGetData(current_node));
        current_node = nodeGetNext(current_node);
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if(!map || !keyElement) {
        iteratorReset(map);
        return MAP_NULL_ARGUMENT;
    }
    Map_node current_node = map -> head_node;
    if(!current_node)
    {
        iteratorReset(map);
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if(map->compareKeyElements(nodeGetKey(current_node),keyElement) == 0)
    {
        map -> head_node = nodeGetNext(current_node);
        destroyNode(map->freeDataElement,map->freeKeyElement,current_node);
        map->size--;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    Map_node next_node = nodeGetNext(current_node);
    while (next_node)
    {
        if (map->compareKeyElements(nodeGetKey(next_node),keyElement) == 0)
        {
            nodeSetNext(current_node,nodeGetNext(next_node));
            destroyNode(map->freeDataElement,map->freeKeyElement,next_node);
            map->size--;
            iteratorReset(map);
            return MAP_SUCCESS;
        }
        current_node = next_node;
        next_node = nodeGetNext(current_node);
    }
    iteratorReset(map);
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map) {
    RETURN_VALUE_IF_TRUE(!map,NULL);
    if(!(map -> head_node))
    {
        iteratorReset(map);
        return NULL;
    }
    map -> iterator_node = map -> head_node;
    return map->copyKeyElement(nodeGetKey(map->head_node));
}

MapKeyElement mapGetNext(Map map) {
    RETURN_VALUE_IF_TRUE(!map,NULL);
    if(!(map -> iterator_node) || !(nodeGetNext(map->iterator_node)))
    {
        iteratorReset(map);
        return NULL;
    }
    map -> iterator_node = nodeGetNext(map->iterator_node);
    return map->copyKeyElement(nodeGetKey(map->iterator_node));
}

MapResult mapClear(Map map) {
    RETURN_VALUE_IF_TRUE(!map,MAP_NULL_ARGUMENT);
    while(map->head_node)
    {
        Map_node temp = map->head_node;
        map->head_node = nodeGetNext(map->head_node);
        destroyNode(map->freeDataElement,map->freeKeyElement,temp);
    }
    map->size = 0;
    return MAP_SUCCESS;   
}
