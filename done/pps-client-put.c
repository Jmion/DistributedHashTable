//
// Created by Simon Wicky on 20.03.18.
//

#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>
#include "node_list.h"
#include "config.h"


int main(void){
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.name = "client";
    init_client.nodes_list = get_nodes();
    client_init(init_client);
    client_t* client = init_client.client;

    do{
        char key[MAX_MSG_ELEM_SIZE+1];
        int read = scanf("%s", key);
        char value[MAX_MSG_ELEM_SIZE+1];
        read += scanf("%s", value);

        if (read == 2) {
            error_code error = network_put(*client, key, value);
            if (error != 0){
                printf("FAIL\n");
            } else {
                printf("OK\n");
            }
        }

        while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');

    }while(!feof(stdin) && ! ferror(stdin));




    return 0;
}