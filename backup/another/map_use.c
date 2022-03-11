


#include "./map_use.h"
#include "stdlib.h"
 


int* toPtr(int num){
    int* ptr = &num;
    return ptr;
}


MapDataElement copyInt(MapDataElement int_ptr){
    if(int_ptr==NULL){
        return NULL;
    }
    int x = *(int*)int_ptr;
    int* new_ptr = &x;
    return (MapDataElement)(new_ptr);
}


/*-----------------------------------------------------------------*/

MapKeyElement copyName(MapKeyElement str_to_copy){
    if(str_to_copy == NULL){
        return NULL;
    }
    char* copy = (char*)malloc( strlen( (char*)str_to_copy ) + 1 );
    copy = strcpy(copy,str_to_copy);
    return (MapKeyElement)copy;
}


/*-----------------------------------------------------------------*/

void deleteInt(MapDataElement int_ptr){
    return;
}


/*-----------------------------------------------------------------*/


void deleteName(MapKeyElement state_name){
    if(state_name == NULL){
        return;
    }
    free( (char*)state_name );
}
/*-----------------------------------------------------------------*/


int compareStates(MapKeyElement state1, MapKeyElement state2){
    if(state1 == NULL || state2 == NULL){
        return 0;
    }
    return strcmp( (char*)state1 ,(char*)state2 );
}







