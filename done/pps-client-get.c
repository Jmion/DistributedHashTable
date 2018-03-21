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
        if (read == 1){

            int value;
            error_code error = network_get(*client, key, &value);
            if (error != 0){
                printf("FAIL\n");
            } else {
                printf("OK %d\n",value);
            }
            while(!feof(stdin) && ! ferror(stdin) && getc(stdin) != '\n');

        }

    }while(!feof(stdin) && ! ferror(stdin));




    return 0;
}