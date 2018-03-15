
#include "error.h"
#include "node.h"
#include "util.h"
#include "system.h"



error_code node_init(node_t *node, const char *ip, uint16_t port, size_t node_id){
	node->ip = ip;
	node->port = port;
	node->id = node_id; 
	return get_server_addr(ip, port,node->address);
}

void node_end(node_t *node){
	
}