//
// Created by Simon Wicky on 20.03.18.
//

#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include "node_list.h"
#include "config.h"
#include "error.h"

#define strtol(x) strtol(x,NULL,10)


int main(int argc,char *argv[]){

    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = get_nodes();
    init_client.argsRequired = TOTAL_SERVERS | PUT_NEEDED | GET_NEEDED;
    char** first = &argv[0];
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;

    size_t nbArgsLeft = argc - (&argv[0] - first);


    if ( nbArgsLeft != 4) {
        debug_print("Wrong number of arguments, %zu arguments", nbArgsLeft);
        printf("FAIL\n");
        client_end(client);
        return 1;
    }


    //checking if enough argument

    //getting key
    pps_value_t value;
    error_code error = network_get(*client, argv[0], &value);

    if (error != ERR_NONE){
        printf("FAIL\n");
        return 0;
    }


    //getting position
    ssize_t position = strtol(argv[1]);
    if (errno != 0) {
    	debug_print("%s", "Conversion of position failed");
        client_end(client);
    	return 1;
    }
    if (position < 0) {
    	position += strlen(value);    	
    }
	if (position >= strlen(value)) {
		printf("FAIL\n");
        client_end(client);
        return 1;
	}


    //getting length
    size_t length = strtol(argv[2]);
    if (errno != 0) {
		debug_print("%s", "Conversion of position failed");
		return 1;
    }
	if (position + length > strlen(value)) {
		printf("FAIL\n");
        client_end(client);
        return 1;
	}	


    char value2[MAX_MSG_ELEM_SIZE+1];
    memset(value2,0,MAX_MSG_ELEM_SIZE+1);
    strncpy(value2, &value[position], length);
    debug_print("New value is '%s'", value2);
    pps_value_t value_get2 = (pps_value_t) value2;
    error = network_put(*client, argv[3], value2);

    if (error != ERR_NONE){
        printf("FAIL\n");
        client_end(client);
        return 1;
    }

	printf("OK\n");

    client_end(client);
    return 0;
}