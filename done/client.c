#include "client.h"
#include "error.h"
#include "system.h"
#include "config.h"


void client_end(client_t *client){
    for (int i = 0; i < client->node_list->size; ++i) {
	    node_end(&client->node_list->nodes[i]);
    }
}

error_code client_init(client_init_args_t client_init){
	client_t* client = client_init.client;
	client->name = client_init.name;
	client->socket = get_socket(1);
	client->node_list = client_init.nodes_list;
	error_code errCode = ERR_NONE;
	if (client->node_list == NULL) {
		errCode = ERR_BAD_PARAMETER;
	}

    M_EXIT_IF_ERR(errCode, "error client_init");

	return errCode;
	
}