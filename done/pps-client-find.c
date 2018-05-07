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


int main(int argc,char *argv[]){

    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = ring_alloc();
    ring_init(init_client.nodes_list);
    init_client.argsRequired = TOTAL_SERVERS | PUT_NEEDED | GET_NEEDED;
    char** first = &argv[0];
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;
  	size_t nbArgsLeft = argc - (&argv[0] - first);

    if ( nbArgsLeft != 2) {
        debug_print("Wrong number of arguments, %zu arguments", nbArgsLeft);
        printf("FAIL\n");
    	client_end(client);
        return 1;
    }


	char value1[MAX_MSG_ELEM_SIZE + 1];
	char value2[MAX_MSG_ELEM_SIZE + 1];
	pps_value_t value_get;	

	error_code error1 = network_get(*client, argv[0], &value_get);
	strcpy(value1, value_get);
	error_code error2 = network_get(*client, argv[1], &value_get);
	strcpy(value2, value_get);

	if (error1 != 0 || error2 != 0) {
		printf("FAIL\n");
		client_end(client);
		return 1;
	}
	
	debug_print("Searching for '%s' in '%s'",value2, value1);
	const char * location = strstr(value1,value2);
	if(location == NULL){
		fprintf(stdout,"OK -1\n");
	}else{
		fprintf(stdout,"OK %ld\n",location - value1);
	}



	client_end(client);
	return 0;
}