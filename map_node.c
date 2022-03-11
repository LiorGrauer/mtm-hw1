#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./util.h"
#include "./mtm_map/map.h"
#include "./map_node.h"

struct Map_node_t {
    MapDataElement data;
    MapKeyElement key;
    Map_node next;
};

Map_node createNode(MapDataElement data, MapKeyElement key) {
    RETURN_VALUE_IF_TRUE(!data || !key, NULL);
	Map_node newNode = malloc(sizeof(*newNode));
    RETURN_VALUE_IF_TRUE(!newNode,NULL);
	newNode->data = data;
	newNode->key = key;
    newNode->next = NULL;
	return newNode;
}

void destroyNode(freeMapDataElements freeDataElement,
                    freeMapKeyElements freeKeyElement, Map_node node) {
    RETURN_VALUE_IF_TRUE(!freeDataElement || !freeKeyElement || !node,);
    freeDataElement(node->data);
    freeKeyElement(node->key);
    free(node);
}

Map_node nodeGetNext(Map_node curNode) {
    RETURN_VALUE_IF_TRUE(!curNode, NULL);
    return curNode->next;
}

MapDataElement nodeGetData(Map_node curNode) {
    RETURN_VALUE_IF_TRUE(!curNode, NULL);
    return curNode->data;
}

MapKeyElement nodeGetKey(Map_node curNode) {
    RETURN_VALUE_IF_TRUE(!curNode, NULL);
    return curNode->key;
}

void nodeSetNext(Map_node curNode, Map_node targetNode) {
    RETURN_VALUE_IF_TRUE(!curNode,);
    curNode->next = targetNode;
}

void nodeSetData(Map_node curNode, MapDataElement targetData) {
    RETURN_VALUE_IF_TRUE(!curNode,);
    curNode->data = targetData;
}

void nodeSetKey(Map_node curNode, MapKeyElement targetKey) {
    RETURN_VALUE_IF_TRUE(!curNode,);
    curNode->key = targetKey;
}

