
#ifndef MAP_USE_H_
#define MAP_USE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./map.h"
 


MapDataElement copyInt(MapDataElement int_ptr); //copyVal
MapKeyElement copyName(MapKeyElement str_to_copy); //copyKey
void deleteInt(MapDataElement int_ptr); //deleteVal
void deleteName(MapKeyElement state_name); //deleteKey
int compareStates(MapKeyElement state1, MapKeyElement state2); //compareKeys
int* toPtr(int num); // returns a pointer to number


#endif