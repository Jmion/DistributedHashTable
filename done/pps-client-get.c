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

    do{
        char key[MAX_MSG_ELEM_SIZE+1];
        int read = scanf("%s", key);
        if (read == 1){

            char value[MAX_MSG_ELEM_SIZE+1];
            pps_value_t value_get = (pps_value_t) value;
            error_code error = network_get(*client, key, &value_get);

            if (error != ERR_NONE){
                printf("FAIL\n");
            // } else if (value_get[0] == '\0') {
            //     printf("FAIL NULL\n");
            } else {
                printf("OK %s\n",value_get);
            }
            while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');

        }

    }while(!feof(stdin) && ! ferror(stdin));




    return 0;
}