#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code);
static Node createNode(int value);
static void destroyList(Node list);
static bool memoryAllocationError (Node node, Node list, ErrorCode* error_code);

int getListLength(Node list) {
    int len=0;
    while(list){
        len++;
        list=list->next;
    }
    return len;
}

bool isListSorted(Node list) {
    while(list && list->next){
        if ((list->x)>(list->next->x))
        {
            return false;
        }
        
    }
    return true;
}

// creates new node with x equals value given
static Node createNode(int value){
    Node node = malloc(sizeof(*node));
    if (!node){ //memory allocation error
        return NULL;
    }
    node->x = value;
    node->next = NULL;
    return node; 
}

// destroys the list completely.
static void destroyList(Node list){
    while(list)
    {
        Node to_destroy = list;
        list = list->next;
        free(to_destroy);
    }
}

//checks if there is memory allocation error in the given node.
//in case of memory allocation error, return true, destroy the list completely, and puts in variable error_code MEMORY_ERROR.
static bool memoryAllocationError (Node node, Node list, ErrorCode* error_code){
    if (!node){
        if(error_code){
            *error_code = MEMORY_ERROR;
        }
        destroyList(list);
        return true;
    }
    return false;
}

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code){
    if (getListLength(list1) == 0 || getListLength(list2) == 0){ //unsorted list
        if(error_code){
            *error_code = NULL_ARGUMENT;
        }
        return NULL;
    }
    if (isListSorted(list1) == 0 || isListSorted(list2) == 0){ //empty list
        if(error_code){
            *error_code = UNSORTED_LIST;
        }
        return NULL;
    } 
    Node merged_list = malloc(sizeof(*merged_list));
    if (memoryAllocationError(merged_list, merged_list, error_code)){ //memory allocation error
        return NULL;
    }
    Node p1 = list1;
    Node p2 = list2;
    Node p_merged = merged_list;

    while(!p1 && !p2){ //there are more nodes on both lists - list 1 and list 2
        if (p1->x < p2->x){
            p_merged = createNode(p1->x);
            if (memoryAllocationError(p_merged, merged_list, error_code)){ //memory allocation error
                return NULL;
            }
            p1 = p1->next;
        }
        else{
            p_merged = createNode(p2->x);
            if (memoryAllocationError(p_merged, merged_list, error_code)){ //memory allocation error
                return NULL;
            }
            p2 = p2->next;
        }
    }
    while (!p1){ //there are more nodes on list 1
        p_merged = createNode(p1->x);
        if (memoryAllocationError(p_merged, merged_list, error_code)){ //memory allocation error
            return NULL;
        }
        p1 = p1->next;
    }
    while (!p2){ //there are more nodes on list 2
        p_merged = createNode(p2->x);
        if (memoryAllocationError(p_merged, merged_list, error_code)){ //memory allocation error
            return NULL;
        }
        p2 = p2->next;
    }
    if(error_code){
        *error_code = SUCCESS;
    }
    return merged_list;
}

int main(){
    ErrorCode* error_code = malloc(sizeof(*error_code));
    *error_code = NULL_ARGUMENT;
    Node list1 = createNode(1);
    list1->next = createNode(4);
    list1->next->next=createNode(9);
    Node list2 = createNode(2);
    list2->next = createNode(5);
    list2->next->next=createNode(8);
    Node merged_list = mergeSortedLists(list1,list2,error_code);
    printf("%d", *error_code);
    while(!merged_list){
        printf("%d",merged_list->x);
        merged_list=merged_list->next;
    }

    return 0;
}