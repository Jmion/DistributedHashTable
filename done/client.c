#include "client.h"
#include "error.h"
#include "system.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>


void client_end(client_t *client){
    for (int i = 0; i < client->node_list->size; ++i) {
	    node_end(&client->node_list->nodes[i]);
    }
    free(client->args);
    client->args = NULL;
}

error_code client_init(client_init_args_t client_init){
	client_t* client = client_init.client;
	client->socket = get_socket(1);
	client->node_list = client_init.nodes_list;
	error_code errCode = ERR_NONE;
	if (client->node_list == NULL) {
		return ERR_BAD_PARAMETER;
	}

    M_EXIT_IF_ERR(errCode, "error client_init");

    //args parsing

	debug_print("%s", "Calling parse_opt_args");
	client->name = (*client_init.argv)[0];
	debug_print("%zu", client_init.argsRequired);

	args_t* args= parse_opt_args(client_init.argsRequired,client_init.argv);
	client->args = args;
	if (args == NULL) {
		return ERR_BAD_PARAMETER;
	}

	return errCode;
	
}