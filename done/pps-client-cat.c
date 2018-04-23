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


int main(void) {
	client_init_args_t init_client;
	client_t cl;
	init_client.client = &cl;
	init_client.name = "client";
	init_client.nodes_list = get_nodes();
	error_code client_init_err = client_init(init_client);
	client_t *client = init_client.client;
	char value[MAX_MSG_ELEM_SIZE + 1];
	memset(value, 0, MAX_MSG_ELEM_SIZE + 1);
	size_t value_len = 0;

	M_EXIT_IF_ERR(client_init_err, "Error initializing client");
		char key1[MAX_MSG_ELEM_SIZE + 1];
		scanf(MAX_MSG_ELEM_SCANF, key1);
	do {
		char key2[MAX_MSG_ELEM_SIZE + 1];
		int read = scanf(MAX_MSG_ELEM_SCANF, key2);
		 if (read == 1) {
		 	pps_value_t value_get;
		 	error_code error = network_get(*client, key1, &value_get);
		 	if (error != 0) {
		 		printf("FAIL\n");
		 		return 0;
		 	} 
			strcpy(&value[value_len], value_get);
		 	value_len = strlen(value);
		}
		strcpy(key1, key2);
		printf("%s\n", value);

		while (!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');

	} while (!feof(stdin) && !ferror(stdin));

	error_code error = network_put(*client, key1, value);

 	if (error == 0) {
		printf("OK\n");
	} else {
		printf("FAIL\n");
	};

	return 0;
}