#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./util.h"
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

Map_node nodeGetNext(Map_node current_node) {
    RETURN_VALUE_IF_TRUE(!current_node, NULL);
    return current_node->next;
}

MapDataElement nodeGetData(Map_node current_node) {
    RETURN_VALUE_IF_TRUE(!current_node, NULL);
    return current_node->data;
}

MapKeyElement nodeGetKey(Map_node current_node) {
    RETURN_VALUE_IF_TRUE(!current_node, NULL);
    return current_node->key;
}

void nodeSetNext(Map_node current_node, Map_node target_node) {
    RETURN_VALUE_IF_TRUE(!current_node,);
    current_node->next = target_node;
}

void nodeSetData(Map_node current_node, MapDataElement target_data) {
    RETURN_VALUE_IF_TRUE(!current_node,);
    current_node->data = target_data;
}

void nodeSetKey(Map_node current_node, MapKeyElement target_key) {
    RETURN_VALUE_IF_TRUE(!current_node,);
    current_node->key = target_key;
}

