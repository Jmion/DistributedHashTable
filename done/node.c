
#include "error.h"
#include "node.h"
#include "util.h"
#include "system.h"
#include <string.h>
#include <stdlib.h>



error_code node_init(node_t *node, const char *ip, uint16_t port, size_t node_id){
	char* ip_copied = calloc(strlen(ip), sizeof(char));
	if (ip_copied == NULL){
		return ERR_NOMEM;
	}
	strncpy(ip_copied, ip, strlen(ip));
	node->ip = ip_copied;
	node->port = port;
	node->id = node_id; 
	return get_server_addr(ip, port,&node->address);
}

void node_end(node_t *node){
	free_const_ptr(node->ip);
}