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
    init_client.nodes_list = get_nodes();
    init_client.argsRequired = TOTAL_SERVERS | GET_NEEDED;
    size_t nbArgsUsed = 0;
    error_code errCode = client_init(init_client, &nbArgsUsed);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;


    if ( argc - 1  - nbArgsUsed != 1) {
        debug_print("Wrong number of arguments, %zu arguments", argc - 1 - nbArgsUsed);
        printf("FAIL\n");
        return 1;
    }



    char value[MAX_MSG_ELEM_SIZE+1];
    pps_value_t value_get = (pps_value_t) value;
    error_code error = network_get(*client, argv[0], &value_get);

    if (error != ERR_NONE){
        printf("FAIL\n");
    } else {
        printf("OK %s\n",value_get);
    }





    return 0;
}