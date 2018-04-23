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


int main(void){
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.name = "client";
    init_client.nodes_list = get_nodes();
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;


    //getting key
    do{
	    char key1[MAX_MSG_ELEM_SIZE+1];
	    pps_value_t value;
	    if (scanf("%s", key1) == 1){

	        error_code error = network_get(*client, key1, &value);

	        if (error != ERR_NONE){
	            printf("FAIL\n");
	            return 0;
	        }


	        while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');

	    } else {
	    	return 0;
	    }
	    //getting position
	    ssize_t position;
	    if (scanf("%zu", &position) == 1) {
	    if (position < 0) {
	    	position += strlen(value);    	
	    }
	    	if (position >= strlen(value)) {
	    		printf("FAIL\n");
	            return 0;
	    	}
	    } else {
	    	return 0;
	    }
	    	while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');


	    //getting length
	    size_t length;
	    if (scanf("%zu", &length) == 1) {
	    	if (position + length >= strlen(value)) {
	    		printf("FAIL\n");
	            return 0;
	    	}
	    } else {
	    	return 0;
	    }
	    	while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');	

	    char key2[MAX_MSG_ELEM_SIZE+1];
	    if (scanf("%s", key2) ==  1){

	        char value2[MAX_MSG_ELEM_SIZE+1];
	        strncpy(value2, &value[position], length);
	        pps_value_t value_get2 = (pps_value_t) value2;
	        error_code error = network_put(*client, key2, value2);

	        if (error != ERR_NONE){
	            printf("FAIL\n");
	            return 0;
	        }



	        while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');

	    	printf("OK\n");
	    } else {
	    	return 0;
	    }
	} while(!feof(stdin) && ! ferror(stdin));

    return 0;
}