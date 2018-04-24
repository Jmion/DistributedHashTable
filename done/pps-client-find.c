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
		char value1[MAX_MSG_ELEM_SIZE + 1];
		char value2[MAX_MSG_ELEM_SIZE + 1];
		int read = scanf(MAX_MSG_ELEM_SCANF, key1);
		char key2[MAX_MSG_ELEM_SIZE + 1];
		read += scanf(MAX_MSG_ELEM_SCANF, key2);
		pps_value_t value_get;	

		if (read == 2) {
			error_code error1 = network_get(*client, key1, &value_get);
			strcpy(value1, value_get);
			error_code error2 = network_get(*client, key2, &value_get);
			strcpy(value2, value_get);
			if (error1 != 0 || error2 != 0) {
				printf("FAIL\n");
				return 0;
			}
			debug_print("Searching for '%s' in '%s'",value2, value1);
			const char * location = strstr(value1,value2);
			if(location == NULL){
				fprintf(stdout,"OK -1\n");
			}else{
				fprintf(stdout,"OK %ld\n",location - value1);
			}
		}

		while (!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');

	} while (!feof(stdin) && !ferror(stdin));


	return 0;
}