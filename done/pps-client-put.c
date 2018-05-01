//
// Created by Simon Wicky on 20.03.18.
//

#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>
#include "node_list.h"
#include "config.h"
#include "error.h"
#include "args.h"


int main(int argc,char *argv[]){
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = get_nodes();
    init_client.argsRequired = TOTAL_SERVERS | PUT_NEEDED;
    size_t nbArgsUsed = 0;
    error_code errCode = client_init(init_client, &nbArgsUsed);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;


    if ( argc - 1  - nbArgsUsed != 2) {
        debug_print("Wrong number of arguments, %zu arguments", argc - 1 - nbArgsUsed);
        return 1;
    }


	error_code error = network_put(*client, argv[0], argv[1]);
	if (error != 0) {
		printf("FAIL\n");
	} else {
		printf("OK\n");
	}



	return 0;
}