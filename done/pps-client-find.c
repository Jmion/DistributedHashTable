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


int main(void) {
	client_init_args_t init_client;
	client_t cl;
	init_client.client = &cl;
	init_client.name = "client";
	init_client.nodes_list = get_nodes();
	error_code client_init_err = client_init(init_client);
	client_t *client = init_client.client;

	M_EXIT_IF_ERR(client_init_err, "Error initializing client");
	do {
		char key1[MAX_MSG_ELEM_SIZE + 1];
		int read = scanf(MAX_MSG_ELEM_SCANF, key1);
		char key2[MAX_MSG_ELEM_SIZE + 1];
		read += scanf(MAX_MSG_ELEM_SCANF, key2);

		if (read == 2) {
			pps_value_t value_get1;
			pps_value_t value_get2;
			debug_print("The key1 : %s",key1);
			debug_print("The key2 : %s",key2);
			error_code error1 = network_get(*client, key1, &value_get1);
			error_code error2 = network_get(*client, key2, &value_get2);
			debug_print("get1 : %s", value_get1);
			debug_print("get2 : %s", value_get2);
			if (error1 != 0 && error2 != 0) {
				printf("FAIL\n");
				return 0;
			}
			debug_print("get1 : %ld",&value_get1);
			debug_print("get2 : %ld",&value_get2);
			debug_print("Searched string : %s",value_get1);
			debug_print("string searched for : %s",value_get2);
			const char * location = strstr(value_get1,value_get2);
			if(location == NULL){
				puts("-1");
			}else{
				debug_print("String start at location : %s",location);
				fprintf(stdout,"OK %ld\n",value_get1-location);
			}
			// 	error_code error = network_put(*client, key, value);
		// 	if (error != 0) {
		// 		printf("FAIL\n");
		// 	} else {
		// 		printf("OK\n");
		// 	}
		}

		while (!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');

	} while (!feof(stdin) && !ferror(stdin));


	return 0;
}