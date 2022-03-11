#ifndef MAP_NODE_H_
#define MAP_NODE_H_

#include "./map.h"

/**
* Generic Map Node
*
* Implements a map node container type.
*
* The following functions are available:
*   createNode  -   Allocates a new node and sets key value and data value 
*   destroyNode -   Deallocates an existing node. Clear it's key and data
*                   by using the stored free functions.
*   nodeGetNext -   Gets node next value.
*   nodeGetData -   Gets node data value.
*   nodeGetKey  -   Gets node key value.
*   nodeSetNext -   Sets node next value.
*   nodeSetData -   Sets node data value.
*   nodeSetKey  -   Sets node key value.
*/

/** Type for defining the map node */
typedef struct Map_node_t *Map_node;

/**
* createNode: Allocates a new node and sets key value and data value 
*   given by user and sets next value to NULL.
*
* @param data - Pointer to MapDataElement that need to set as the node data value.
* @param key - Pointer to MapKeyElement that need to set as the node key value.
*
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Node in case of success.
*/
Map_node createNode(MapDataElement data, MapKeyElement key);

/**
* destroyNode: Deallocates an existing node. Clear it's key and data by using the
* stored free functions.
*
* @param freeDataElement - Function pointer to be used for removing data elements from the map
* @param freeKeyElement - Function pointer to be used for removing key elements from the map
* @param node - Pointer to the node that needs to be destroyed.
*/
void destroyNode(freeMapDataElements freeDataElement,freeMapKeyElements freeKeyElement, Map_node node);

/**
* nodeGetNext: Gets node next value.
*
* @param current_node - Pointer to the node that it's key value is requested.
*
* @return
* 	NULL - if one of the parameter is NULL.
* 	The node key value in case of success
*/
Map_node nodeGetNext(Map_node current_node);

/**
* nodeGetData: Gets node data value.
* @param current_node - Pointer to the node that it's key value is requested.
*
* @return
* 	NULL - if one of the parameter is NULL.
* 	The node key value in case of success
*/
MapDataElement nodeGetData(Map_node current_node);

/**
* nodeGetKey: Gets node key value.
*
* @param current_node - Pointer to the node that it's key value is requested.
*
* @return
* 	NULL - if one of the parameter is NULL.
* 	The node key value in case of success
*/
MapKeyElement nodeGetKey(Map_node current_node);

/**
* nodeSetNext: Sets node next value.
*
* @param current_node - Pointer to the node that it's next value is being set.
* @param target_node - Node pointer that need to be inserted to the node next value.
*/
void nodeSetNext(Map_node current_node, Map_node target_node);

/**
* nodeSetData: Sets node data value.
*
* @param current_node - Pointer to the node that it's data value is being set.
* @param target_data - Data value that need to be set for the node.
*/
void nodeSetData(Map_node current_node, MapDataElement target_data);

/**
* nodeSetKey: Sets node key value.
*
* @param current_node - Pointer to the node that it's key value is being set.
* @param target_key - Key value that need to be set for the node.
*/
void nodeSetKey(Map_node current_node, MapKeyElement target_key);


#endif /*MAP_NODE_H_*/