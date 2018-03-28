//
// Created by Simon Wicky on 27.03.18.
//

#include "node_list.h"
#include "node.h"
#include "error.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#define IP_SIZE 15


node_list_t *node_list_new(){
    size_t size = 0;
    node_list_t* list = calloc(1,sizeof(node_list_t));
    if (list == NULL) {
        return NULL;
    }
    node_t* nodes = NULL;
    list->nodes = nodes;
    list->size = size;
    return list;
}

node_list_t *get_nodes(){
    node_list_t* list = node_list_new();
    if (list == NULL) {
        debug_print("%s\n","Error initialising node list");
        return NULL;
    }
    FILE* in = fopen(PPS_SERVERS_LIST_FILENAME, "r");
    if (in == NULL) {
        debug_print("%s\n","Lecture error");
        return NULL;
    } else {
        char IP[IP_SIZE];
        int port = 0;
        size_t index = 0;
        while(!feof(in) && ! ferror(in)){
            fscanf(in, "%s", IP);
            fscanf(in, "%d", &port);
            if (port <= 0 || port > 65535) {
                return NULL;
            }
            list->nodes = realloc(list->nodes, list->size + 1);
            list->size += 1;
            if (list->nodes == NULL) {
                return NULL;
            }
            if(node_init(&list->nodes[index], IP, port, index) != ERR_NONE){
                debug_print("%s\n","Error initialising node");
                return NULL;
            }    
            ++index;

        }
    }
    return list;
}

error_code node_list_add(node_list_t *list, node_t node){
    if (list == NULL || node == NULL) {
        return ERR_BAD_PARAMETER;
    }
    if(list->size >= SIZE_MAX/ sizeof(node)){
        return ERR_NOMEM;
    }
    node_t * oldNodeList = list->nodes;
    list->size++;
    if(list = realloc(list,((list->size)+1)* sizeof(node)) == NULL){
        list->size--;
        list->nodes = oldnodeList;
        return ERR_NOMEM;
    }
    return ERR_NONE;
}


void node_list_sort(node_list_t *list, int (*comparator)(const node_t *, const node_t *)){
    //not yet to be implemented
}


void node_list_free(node_list_t *list){
    for (int i = 0; i < list->size; ++i) {
        node_end(&list->nodes[i]);
    }
    free(list->nodes);
    list->nodes = NULL;
    free(list);
    list = NULL;
}