#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "./map.h"

typedef struct node {
    MapDataElement data;
    MapKeyElement key;
    struct node* next;
} *Node;

struct Map_t {
    Node headNode;
    int size;
    Node iteratorNode;
    MapKeyElement iteratorKey;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

static Node createNode(MapDataElement data, MapKeyElement key) {
	Node newNode = malloc(sizeof(*newNode));
    RETURN_VALUE_IF_TRUE(!newNode,NULL);
	newNode->data = data;
	newNode->key = key;
    newNode->next = NULL;
	return newNode;
}

static void destroyNode(Map map, Node node) {
    RETURN_VALUE_IF_TRUE(!map || !node,);
    map->freeDataElement(node->data);
    map->freeKeyElement(node->key);
    free(node);
}

static void iteratorReset(Map map) {
    RETURN_VALUE_IF_TRUE(!map,);
    map -> iteratorNode = NULL;
    map -> iteratorKey = NULL;
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
    map -> iteratorKey = NULL;
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
    Node curNode = map -> headNode;
    while (curNode) {
        RETURN_VALUE_IF_TRUE(map -> compareKeyElements(curNode->key,element) == 0,true);
        curNode = curNode->next;
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if(!map || !keyElement || !dataElement) {
        iteratorReset(map);
        return MAP_NULL_ARGUMENT;
    }
    Node curNode = map -> headNode;
    MapDataElement dataCopy= map->copyDataElement(dataElement);
    if(!dataCopy) {
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    MapDataElement keyCopy= map->copyKeyElement(keyElement);
    if(!keyCopy) {
        map->freeDataElement(dataCopy);
        iteratorReset(map);
        return MAP_OUT_OF_MEMORY;
    }
    Node newNode = createNode(dataCopy,keyCopy);
    if(!newNode) {
        map->freeDataElement(curNode->data);
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
            if (map->compareKeyElements(curNode->key,keyCopy) == 0)
            {
                free(newNode);
                map->freeDataElement(curNode->data);
                map->freeKeyElement(keyCopy);
                curNode->data = dataCopy;
                iteratorReset(map);
                return MAP_SUCCESS;
            }
            curNode= curNode->next;
        }
    }
    if(map->compareKeyElements(curNode->key,keyCopy) > 0)
    {
        map->headNode = newNode;
        newNode->next = curNode;
        map->size++;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    Node NextNode = curNode -> next;
    while (NextNode)
    {
        if (map->compareKeyElements(NextNode->key,keyCopy) > 0)
        {
            curNode->next = newNode;
            newNode->next = NextNode;
            map->size++;
            iteratorReset(map);
            return MAP_SUCCESS;
        }
        curNode = NextNode;
        NextNode = curNode->next;
    }
    curNode->next = newNode;
    newNode->next = NextNode;
    map->size++;
    iteratorReset(map);
    return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    RETURN_VALUE_IF_TRUE(!map || !keyElement,NULL);
    Node curNode = map -> headNode;
    while (curNode)
    {
        RETURN_VALUE_IF_TRUE(map -> compareKeyElements(curNode->key,keyElement) == 0,curNode->data);
        curNode = curNode->next;
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if(!map || !keyElement) {
        iteratorReset(map);
        return MAP_NULL_ARGUMENT;
    }
    Node curNode = map -> headNode;
    if(!curNode)
    {
        iteratorReset(map);
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if(map->compareKeyElements(curNode->key,keyElement) == 0)
    {
        map -> headNode = curNode->next;
        destroyNode(map,curNode);
        map->size--;
        iteratorReset(map);
        return MAP_SUCCESS;
    }
    Node NextNode = curNode -> next;
    while (NextNode)
    {
        if (map->compareKeyElements(NextNode->key,keyElement) == 0)
        {
            curNode->next = NextNode->next;
            destroyNode(map,NextNode);
            map->size--;
            iteratorReset(map);
            return MAP_SUCCESS;
        }
        curNode = NextNode;
        NextNode = curNode->next;
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
    map -> iteratorKey = map -> headNode -> key;
    return map->iteratorKey;
}

MapKeyElement mapGetNext(Map map) {
    RETURN_VALUE_IF_TRUE(!map || map->iteratorKey == NULL,NULL);
    if(!(map -> iteratorNode -> next))
    {
        iteratorReset(map);
        return NULL;
    }
    map -> iteratorNode = map -> iteratorNode -> next;
    map -> iteratorKey = map -> iteratorNode -> key;
    return map->iteratorKey;
}

MapResult mapClear(Map map) {
    RETURN_VALUE_IF_TRUE(!map,MAP_NULL_ARGUMENT);
    while(map->headNode)
    {
        Node temp = map->headNode;
        map->headNode = map->headNode->next;
        destroyNode(map, temp);
    }
    map->size = 0;
    return MAP_SUCCESS;   
}
