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
    init_client.nodes_list = ring_alloc();
    ring_init(init_client.nodes_list);
    init_client.argsRequired = TOTAL_SERVERS | PUT_NEEDED;
    char** first = &argv[0];
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;
    size_t nbArgsLeft = argc - (&argv[0] - first);

    if ( nbArgsLeft != 2) {
        debug_print("Wrong number of arguments, %zu arguments", nbArgsLeft);
        printf("FAIL\n");
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