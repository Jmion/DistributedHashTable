#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>
#include "node_list.h"
#include "config.h"
#include "args.h"
#include "error.h"
#include "system.h"

#define TRUE 1
#define FALSE 0

void printsha(unsigned char sha[]) {
    printf("(");
    for (size_t i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        printf("%02x", sha[i]);
    }
    printf(")");
}

int main(int argc, char *argv[]) {
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = ring_alloc();
    ring_init(init_client.nodes_list);
    init_client.argsRequired = 0;
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode, "Error initializing client");
    client_t* client = init_client.client;
    int socket = get_socket(1);

    char* msg[1];
    char* buffer[1];

    //sort the list
    node_list_sort(client->node_list, node_cmp_server_addr);
    size_t flags[client->node_list->size];
    memset(flags, 0, client->node_list->size);

    //sending
    for (size_t i = 0; i < client->node_list->size; ++i) {
        size_t flag = 1;
        if (sendto(socket, msg, 0, 0, (struct sockaddr *) &client->node_list->nodes[i].address, sizeof(client->node_list->nodes[i].address)) == -1) {
            debug_print("%s", "Sending failed.");
            flag = -1;
        }
        flags[i] = flag;
    }
    //receiving
    for (size_t i = 0; i < client->node_list->size; ++i) {
        ssize_t flag;
        struct sockaddr_in serv_address;
        socklen_t sock_len = sizeof(serv_address);
        if (recvfrom(socket, buffer, 1, 0, (struct sockaddr*) &serv_address, &sock_len) == -1) {
            debug_print("%s", "Receiving failed.");
            flag = -1;
        } else {
            flag = 1;
        }

        //checking who replied
        size_t j = 0;
        int found = FALSE;
        while(j < client->node_list->size && !found) {
            if (client->node_list->nodes[j].address.sin_port == serv_address.sin_port && client->node_list->nodes[j].address.sin_addr.s_addr == serv_address.sin_addr.s_addr ) {
                if (flags[j] == 1 && flag == 1) {
                    flags[j] = 2;
                    found = TRUE; 
                }
            }
            ++j;
        }

    }

    //printing status
    for (size_t i = 0; i < client->node_list->size; ++i) {
        printf("%s %d ", client->node_list->nodes[i].ip, client->node_list->nodes[i].port);
        printsha(client->node_list->nodes[i].SHA);
        printf(" ");
        if (flags[i] == 2) {
            printf("OK\n");
        } else {
            printf("FAIL\n");
        }
    }



    client_end(client);
    return 0;
}