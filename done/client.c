#include "client.h"
#include "error.h"
#include "system.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "args.h"


void client_end(client_t *client){
    for (int i = 0; i < client->node_list->size; ++i) {
	    node_end(&client->node_list->nodes[i]);
    }
    free(client->args);
    client->args = NULL;
}

error_code client_init(client_init_args_t client_init, size_t* nbArgsUsed){
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
	++(*client_init.argv);

	char** first = &(*client_init.argv)[0];

	args_t* args= parse_opt_args(client_init.argsRequired,client_init.argv);
	client->args = args;
	//number of args used
	if (nbArgsUsed != NULL) {
		*nbArgsUsed = &(*client_init.argv)[0] - first;
	}
	if (args == NULL) {
		return ERR_BAD_PARAMETER;
	}

	return errCode;
	
}