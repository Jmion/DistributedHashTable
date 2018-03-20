#include "client.h"
#include "error.h"
#include "system.h"
#include "config.h"


void client_end(client_t *client){
	node_end(&client->node);
}

error_code client_init(client_init_args_t client_init){
	client_t* client = client_init.client;
	client->name = client_init.name;
	client->socket = get_socket(1);
	node_init(&client->node, PPS_DEFAULT_IP,PPS_DEFAULT_PORT,1);

    //handle error case
	return ERR_NONE;
	
}