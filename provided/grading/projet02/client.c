#include "client.h"
#include "error.h"
#include "system.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "args.h"


void client_end(client_t *client){
	node_list_free(client->node_list);
    free(client->args);
    client->args = NULL;
	//correcteur: free(client) ?
}

error_code client_init(client_init_args_t client_init){
	client_t* client = client_init.client;
	client->socket = get_socket(1);
	//correcteur: get_nodes ?
	client->node_list = client_init.nodes_list;
	error_code errCode = ERR_NONE;
	if (client->node_list == NULL) {
		return ERR_BAD_PARAMETER;
	}

	//correcteur: errCode inutile
    M_EXIT_IF_ERR(errCode, "error client_init");

    //args parsing

	client->name = (*client_init.argv)[0];
	++(*client_init.argv);

	args_t* args= parse_opt_args(client_init.argsRequired,client_init.argv);
	client->args = args;
	if (args == NULL) {
		return ERR_BAD_PARAMETER;
	}

	//correcteur: faire dans parse_opt_args
	args->N = args->N > client->node_list->size ? client->node_list->size : args->N;
	args->R = args->R > args->N ? args->N : args->R;
	args->W = args->W > args->N ? args->N : args->W;

	return errCode;

}
