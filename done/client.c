#include "client.h"


void client_end(client_t *client){
	client.node.end();
}

error_code client_init(client_init_args_t client_init){
	client_init.client.name = client_init.name;
	//wip
}