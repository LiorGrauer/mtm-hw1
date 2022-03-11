#ifndef MAP_NODE_H_
#define MAP_NODE_H_

typedef struct Map_node_t *Map_node;

Map_node createNode(MapDataElement data, MapKeyElement key);

void destroyNode(freeMapDataElements freeDataElement,freeMapKeyElements freeKeyElement, Map_node node);

Map_node nodeGetNext(Map_node curNode);

MapDataElement nodeGetData(Map_node curNode);

MapKeyElement nodeGetKey(Map_node curNode);

void nodeSetNext(Map_node curNode, Map_node TargetNode);

void nodeSetData(Map_node curNode, MapDataElement targetData);

void nodeSetKey(Map_node curNode, MapKeyElement targetKey);


#endif /*MAP_NODE_H_*/