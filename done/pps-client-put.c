//
// Created by Simon Wicky on 20.03.18.
//

#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>


int main(void){
    client_init_args_t init_client;
    init_client.name = "client";
    client_init(init_client);
    client_t* client = init_client.client;

    do{
        char key;
        int read = scanf("%c", &key);
        int value;
        read += scanf("%d", &value);
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