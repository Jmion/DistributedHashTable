#include "ring.h"
#include "error.h"
#include <stdlib.h>
#include "node_list.h"

ring_t* ring_alloc(){
	ring_t* ring = calloc(1,sizeof(ring_t));
	if (ring == NULL){
		debug_print("%s", "Allocation error");
	}
	return ring;
}


error_code ring_init(ring_t* ring){
	ring_t* list = get_nodes();
	node_list_sort(list, node_cmp_sha);
	if (list == NULL){
		return ERR_BAD_PARAMETER;
	}
	*ring = *list;
	return ERR_NONE;

}

node_list_t *ring_get_nodes_for_key(const ring_t *ring, size_t wanted_list_size, pps_key_t key){
	//TODO implement this
	return NULL;
}