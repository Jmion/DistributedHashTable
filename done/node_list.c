#include "node_list.h"
#include "node.h"
#include "error.h"
#include "config.h"
#include <stdio.h>
#include <stdint.h>
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
        char IP[IP_SIZE+1];
        memset(&IP,0, sizeof(IP));
        int port = 0;
        size_t index = 0;
        while(!feof(in) && ! ferror(in)){
            int read = 0;
            read += fscanf(in, "%s", IP);
            read += fscanf(in, "%d", &port);
            read += fscanf(in, "%zu", &index);
            if (read == 3){
                if (port <= 0 || port > 65535) {
                    return NULL;
                }
                for (size_t i = 1; i <= index; ++i) {
                    node_t tempNode;
                    memset(&tempNode, 0, sizeof(node_t));
                    if(node_init(&tempNode, IP, port, i) != ERR_NONE){
                        debug_print("%s\n","Error initialising node");
                        return NULL;
                    }

                    if(node_list_add(list,tempNode) != ERR_NONE){
                        debug_print("%s\n", "issue with adding a node to the list.");
                        return NULL;
                    }
                }
            }
        }
    }
    fclose(in);
    return list;
}

//Modular
#define ENLARGE_PADDING 4
node_list_t* node_list_enlarge(node_list_t* list) {
	node_list_t *result = list;
	if (result != NULL) {
		node_t *oldNode = list->nodes;
		result->allocated += ENLARGE_PADDING;
		//debug_print("Allocated is %zu",result->allocated);
		if ((result->allocated > SIZE_MAX / sizeof(node_t)) ||
		    ((result->nodes = realloc(result->nodes, result->allocated * sizeof(node_t))) == NULL)) {
			result->nodes = oldNode;
			result->allocated -= ENLARGE_PADDING;
			result = NULL;
		}
	}
	return result;
}


error_code node_list_add(node_list_t *list, node_t const node){
	M_REQUIRE_NON_NULL(list);
	//debug_print("The size is : %zu and the allocated size is : %zu", list->size, list->allocated);
	while(list->size >= list->allocated) {
		if (node_list_enlarge(list) == NULL) {
			return ERR_NOMEM;
		}
	}
	list->nodes[list->size] = node;
	++(list->size);
    return ERR_NONE;
}





void node_list_sort(node_list_t *list, int (*comparator)(const node_t *, const node_t *)){
    qsort(list->nodes, list->size, sizeof(*list->nodes), (int(*) (const void*, const void*)) comparator);
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