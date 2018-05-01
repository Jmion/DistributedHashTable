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
#include <string.h>


int main(int argc,char *argv[]){
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = get_nodes();
    init_client.argsRequired = TOTAL_SERVERS | PUT_NEEDED | GET_NEEDED;
    size_t nbArgsUsed = 0;
    error_code errCode = client_init(init_client, &nbArgsUsed);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;
    size_t nbArgsLeft = argc - 1 - nbArgsUsed;


    if ( nbArgsLeft < 2) {
        debug_print("%s","Not enough argument");
        return 1;
    }

	char value[MAX_MSG_ELEM_SIZE + 1];
	memset(value, 0, MAX_MSG_ELEM_SIZE + 1);
	size_t value_len = 0;
	error_code error = 0;

	for (size_t i = 0; i < nbArgsLeft - 1; ++i) {
	 	pps_value_t value_get;
	 	error += network_get(*client, argv[i], &value_get);
	 	if (error == 0) {
			strcpy(&value[value_len], value_get);
		 	value_len = strlen(value);
	 	} 
		debug_print("New value is currently '%s'. Error code is %d", value, error);
	}

	error += network_put(*client, argv[nbArgsLeft], value);

 	if (error == 0) {
		printf("OK\n");
	} else {
		printf("FAIL\n");
	};

	return 0;
}