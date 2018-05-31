#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>
#include "node_list.h"
#include "config.h"
#include "args.h"
#include "error.h"


int main(int argc,char *argv[]){
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = get_nodes();
    init_client.argsRequired = 0;
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode,"Error initializing client");
    client_t* client = init_client.client;

    char* msg[1];
    char* buffer[1];

    for (int i = 0; i < client->node_list->size; ++i) {
        size_t flag = 1;
        if (sendto(client->socket, msg, 0, 0, (struct sockaddr *) &client->node_list->nodes[i].address, sizeof(client->node_list->nodes[i].address)) == -1){
            debug_print("%s", "Sending failed.");
            flag = -1;
        }
        //correcteur: utiliser recvfrom et verifier la provenance du message
        flag =  recv(client->socket, buffer, 1, 0);
        //correcteur: pas print dans stdout
        printf("%s %d ", client->node_list->nodes[i].ip,client->node_list->nodes[i].port);
        if(flag == 0){
            printf("OK\n");
        } else {
            printf("FAIL\n");
        }
    }



    client_end(client);
    return 0;
}
