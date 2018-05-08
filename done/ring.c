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

	for (size_t i = 0; i < wanted_list_size; ++i) {
		node_list_add(list,ring->nodes[(index + i) % ring->size]);
	}


	return list;
}