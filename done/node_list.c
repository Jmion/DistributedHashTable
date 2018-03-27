//
// Created by Simon Wicky on 27.03.18.
//

#include "node_list.h"
#include "node.h"


node_list_t *node_list_new(){
    size_t size = 10; //arbitrary how much?
    node_list_t* list;
    node_t* nodes = malloc(size);
    list->nodes = nodes;
    list->size = size;
    return list;
}

node_list_t *get_nodes(){

}

error_code node_list_add(node_list_t *list, node_t node){

}


void node_list_sort(node_list_t *list, int (*comparator)(const node_t *, const node_t *)){
    //not yet to be implemented
}


void node_list_free(node_list_t *list){
    for (int i = 0; i < list->size; ++i) {
        node_end(&nodes[i]);
    }
    free(list->nodes);
    list->nodes == NULL;
}