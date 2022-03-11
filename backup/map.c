#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./util.h"
#include "./mtm_map/map.h"
#include "./map_node.h"

struct Map_t {
    Map_node headNode;
    int size;
    Map_node iteratorNode;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

static void iteratorReset(Map map) {
    RETURN_VALUE_IF_TRUE(!map,);
    map -> iteratorNode = NULL;
}

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements) 
{
    RETURN_VALUE_IF_TRUE(!copyDataElement || !copyKeyElement || !freeDataElement || !freeKeyElement || !compareKeyElements,NULL);
    Map map = malloc(sizeof(*map));
    RETURN_VALUE_IF_TRUE(!map,NULL);
    map -> headNode = NULL;
    map -> size = 0;
    map -> iteratorNode = NULL;
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
    MapKeyElement curKey = mapGetFirst(map);
    MapDataElement curData = mapGet(map,curKey);
    while(curKey) {
        RETURN_VALUE_IF_TRUE(mapPut(mapCopy,curKey,curData)==MAP_OUT_OF_MEMORY,NULL);
        map->freeKeyElement(curKey);
        curKey = mapGetNext(map);
        curData = mapGet(map,curKey);
    }
    return mapCopy;
}

int mapGetSize(Map map) {
    if(!map) {
        return -1;
    }
    return map -> size;
}

bool mapContains(Map map, MapKeyElement element) {
    RETURN_VALUE_IF_TRUE(!map || !element,false);
    Map_node curNode = map -> headNode;
    while (curNode) {
        RETURN_VALUE_IF_TRUE(map -> compareKeyElements(nodeGetKey(curNode),element) == 0,true);
        curNode = nodeGetNext(curNode);
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if(!map || !keyElement || !dataElement) {
        iteratorReset(map);
        return MAP_NULL_ARGUMENT;
    }
    Map_node curNode = map -> headNode;
    MapDataElement dataCopy= map->copyDataElement(dataElement);
    if(!dataCopy) {
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    MapKeyElement keyCopy= map->copyKeyElement(keyElement);
    if(!keyCopy) {
        map->freeDataElement(dataCopy);
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    Map_node newNode = createNode(dataCopy,keyCopy);
    if(!newNode) {
        map->freeDataElement(nodeGetData(curNode));
        map->freeKeyElement(keyCopy);
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    if(!curNode)
    {
        map->headNode = newNode;
        map->size++;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    if(mapContains(map,keyCopy)) {
        while (curNode) {
            if (map->compareKeyElements(nodeGetKey(curNode),keyCopy) == 0)
            {
                free(newNode);
                map->freeDataElement(nodeGetData(curNode));
                map->freeKeyElement(keyCopy);
                nodeSetData(curNode,dataCopy);
                iteratorReset(map);
                return MAP_SUCCESS;
            }
            curNode= nodeGetNext(curNode);
        }
    }
    if(map->compareKeyElements(nodeGetKey(curNode),keyCopy) > 0)
    {
        map->headNode = newNode;
        nodeSetNext(newNode,curNode);
        map->size++;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    Map_node NextNode = nodeGetNext(curNode);
    while (NextNode)
    {
        if (map->compareKeyElements(nodeGetKey(NextNode),keyCopy) > 0)
        {
            nodeSetNext(curNode,newNode);
            nodeSetNext(newNode,NextNode);
            map->size++;
            iteratorReset(map);
            return MAP_SUCCESS;
        }
        curNode = NextNode;
        NextNode = nodeGetNext(curNode);
    }
    nodeSetNext(curNode,newNode);
    nodeSetNext(newNode,NextNode);
    map->size++;
    iteratorReset(map);
    return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    RETURN_VALUE_IF_TRUE(!map || !keyElement,NULL);
    Map_node curNode = map -> headNode;
    while (curNode)
    {
        RETURN_VALUE_IF_TRUE(map -> compareKeyElements(nodeGetKey(curNode),keyElement) == 0,nodeGetData(curNode));
        curNode = nodeGetNext(curNode);
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if(!map || !keyElement) {
        iteratorReset(map);
        return MAP_NULL_ARGUMENT;
    }
    Map_node curNode = map -> headNode;
    if(!curNode)
    {
        iteratorReset(map);
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if(map->compareKeyElements(nodeGetKey(curNode),keyElement) == 0)
    {
        map -> headNode = nodeGetNext(curNode);
        destroyNode(map->freeDataElement,map->freeKeyElement,curNode);
        map->size--;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    Map_node NextNode = nodeGetNext(curNode);
    while (NextNode)
    {
        if (map->compareKeyElements(nodeGetKey(NextNode),keyElement) == 0)
        {
            nodeSetNext(curNode,nodeGetNext(NextNode));
            destroyNode(map->freeDataElement,map->freeKeyElement,NextNode);
            map->size--;
            iteratorReset(map);
            return MAP_SUCCESS;
        }
        curNode = NextNode;
        NextNode = nodeGetNext(curNode);
    }
    iteratorReset(map);
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map) {
    RETURN_VALUE_IF_TRUE(!map,NULL);
    if(!(map -> headNode))
    {
        iteratorReset(map);
        return NULL;
    }
    map -> iteratorNode = map -> headNode;
    return map->copyKeyElement(nodeGetKey(map->headNode));
}

MapKeyElement mapGetNext(Map map) {
    RETURN_VALUE_IF_TRUE(!map,NULL);
    if(!(map -> iteratorNode) || !(nodeGetNext(map->iteratorNode)))
    {
        iteratorReset(map);
        return NULL;
    }
    map -> iteratorNode = nodeGetNext(map->iteratorNode);
    return map->copyKeyElement(nodeGetKey(map->iteratorNode));
}

MapResult mapClear(Map map) {
    RETURN_VALUE_IF_TRUE(!map,MAP_NULL_ARGUMENT);
    while(map->headNode)
    {
        Map_node temp = map->headNode;
        map->headNode = nodeGetNext(map->headNode);
        destroyNode(map->freeDataElement,map->freeKeyElement,temp);
    }
    map->size = 0;
    return MAP_SUCCESS;   
}
