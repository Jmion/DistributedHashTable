#include "client.h"
#include "node.h"
#include "system.h"


void client_end(client_t *client){
	node_end(&client->node);
}

error_code client_init(client_init_args_t client_init){
	client_t* client = client_init.client;
	client->name = client_init.name;
	client->socket = get_socket(1);
	//node_init(client.&node, ???);

	return ERR_BAD_PARAMETER;
	
}