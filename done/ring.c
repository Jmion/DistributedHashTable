#include "ring.h"
#include "error.h"
#include <stdlib.h>
#include "node_list.h"
#include <openssl/sha.h>
#include "node.h"

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
	node_list_t* list = node_list_new();
	size_t index = 0;
	unsigned char sha[SHA_DIGEST_LENGTH];
	SHA1((const unsigned char* )key, strlen(key),sha);
	node_t artificial_node;
	memset(&artificial_node, 0, sizeof(node_t));
	artificial_node.SHA = sha;
	while(node_cmp_sha(&ring->nodes[index], &artificial_node) < 0){
		++index;
	}
	size_t nbNodes = 0;
	size_t i = 0;
	while (nbNodes < wanted_list_size && i < ring->size) {
    	size_t flag = 0;
    	for (size_t j = 0; j < list->size; ++j) {
    		if (node_cmp_server_addr(&list->nodes[j],&ring->nodes[(index + i) % ring->size]) == 0){
    			flag = 1;
    		}
    	}
    	if (flag != 1){

		#ifdef DEBUG
			fprintf(stderr,"%s %d ", ring->nodes[(index + i) % ring->size].ip, ring->nodes[(index + i) % ring->size].port);
		    fprintf(stderr,"(");
    		for (int j = 0; j < SHA_DIGEST_LENGTH; ++j){
        		fprintf(stderr,"%02x", ring->nodes[(index + i) % ring->size].SHA[j]);
    		}
    		fprintf(stderr,")\n");
		#endif

    		node_list_add(list,ring->nodes[(index + i) % ring->size]);
    		++nbNodes;
    	}
    	++i;
	}

	return list;
}