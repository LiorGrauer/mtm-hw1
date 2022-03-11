
#include "./map.h"
#include "./map_use.h"

int main(){
    Map map1 = mapCreate( &copyInt, &copyName, &deleteInt, &deleteName,  &compareStates);
    mapPut(map1,"Washington", toPtr(1053) );
    mapPut(map1,"Florida", toPtr(153) );
    Map map2 = mapCopy(map1);
    mapDestroy(map1);
    mapDestroy(map2);
    return 0;
}