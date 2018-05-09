#include "ring.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>
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

void print_sha(unsigned char sha[]){
    fprintf(stderr,"(");
	for (int j = 0; j < SHA_DIGEST_LENGTH; ++j){
		fprintf(stderr,"%02x", sha[j]);
	}
	fprintf(stderr,")");
}

error_code ring_init(ring_t* ring){
	ring_t* list = get_nodes();
	node_list_sort(list, node_cmp_sha);
	if (list == NULL){
		return ERR_BAD_PARAMETER;
	}
	*ring = *list;
//-------------------Debug block--------------------------
	#ifdef DEBUG
	for (size_t i = 0; i < ring->size; ++i) {
		fprintf(stderr,"%s %d ", ring->nodes[i].ip, ring->nodes[i].port);
		print_sha(ring->nodes[i].SHA);
		fprintf(stderr, "\n");
	}
	#endif
//------------------- End Debug block--------------------------  

	return ERR_NONE;

}

void ring_free(ring_t* ring){
	node_list_free(ring);
}


node_list_t *ring_get_nodes_for_key(const ring_t *ring, size_t wanted_list_size, pps_key_t key){
	node_list_t* list = node_list_new();
	size_t index = 0;
	unsigned char sha[SHA_DIGEST_LENGTH];
	SHA1((const unsigned char* )key, strlen(key),sha);

//-------------------Debug block--------------------------
	#ifdef DEBUG
	debug_print("%s","SHA of key is : ");
	print_sha(sha);
	fprintf(stderr, "\n");
	#endif
//------------------- End Debug block--------------------------  

	node_t artificial_node;
	memset(&artificial_node, 0, sizeof(node_t));
	artificial_node.SHA = sha;
	while(node_cmp_sha(&ring->nodes[index], &artificial_node) < 0){
		++index;
	}
	size_t nbNodes = 0;
	size_t i = 0;
	debug_print("%s", "Key added to following nodes : ");
	while (nbNodes < wanted_list_size && i < ring->size) {
    	size_t flag = 0;
		size_t index_ring = (index + i) % ring->size;
    	for (size_t j = 0; j < list->size; ++j) {
    		if (strcmp(list->nodes[j].ip,ring->nodes[index_ring].ip) == 0 && list->nodes[j].port == ring->nodes[index_ring].port){
    			flag = 1;
    		}
    	}
    	if (flag != 1){

//-------------------Debug block--------------------------
    		#ifdef DEBUG
			fprintf(stderr,"%s %d ", ring->nodes[index_ring].ip, ring->nodes[index_ring].port);
    		print_sha(ring->nodes[index_ring].SHA);
			fprintf(stderr, "\n");
    		#endif
//------------------- End Debug block--------------------------   

    		node_list_add(list,ring->nodes[index_ring]);
    		++nbNodes;
    	}
    	++i;
	}

	return list;
}