#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define IP_SIZE 15
#define UDP_MAX_SIZE 65507
#define strtol(x) strtol(x, NULL, 10)


int main(int argc, char *argv[]) {
    if (argc != 3) {
        debug_print("%s", "Wrong number of arguments");
        printf("FAIL\n");
        return 1;
    }
    int port = strtol(argv[2]);
    if (port == 0) {
        debug_print("%s", "Couldn't parse port number");
        printf("FAIL\n");
        return 1;
    }

    struct sockaddr_in address;
    error_code err = get_server_addr(argv[1], port, &address);
    if (err != ERR_NONE){
        debug_print("%s", "Failed getting server_addr.");
        return 1;
    }
    int socket = get_socket(1);
    //correcteur: check error socket
    char msg = '\0';
    if (sendto(socket, &msg, 1, 0, (struct sockaddr *) &address, sizeof(address)) == -1) {
        debug_print("%s", "Sending failed.");
        fprintf(stdout, "FAIL\n");
        return 1;
    }

    unsigned int nbAnswers = 1;
    char buffer[UDP_MAX_SIZE];
    size_t msg_length;
    struct sockaddr_in serv_address;
    socklen_t sock_len = sizeof(serv_address);

    msg_length = recvfrom(socket, &buffer, UDP_MAX_SIZE, 0,(struct sockaddr*) &serv_address, &sock_len);
    if ( msg_length == -1 || address.sin_port != serv_address.sin_port || address.sin_addr.s_addr != serv_address.sin_addr.s_addr) {
        debug_print("%s", "No answers");
        fprintf(stdout, "FAIL\n");
        return 1;
    }

    memcpy(&nbAnswers, buffer, sizeof(unsigned int));
    nbAnswers = ntohl(nbAnswers);
    size_t index = sizeof(unsigned int);

    while (nbAnswers > 0) {
        if (index < msg_length) {
            printf("%s", &buffer[index]);
            index += strlen(&buffer[index]) + 1;
            printf(" = %s\n", &buffer[index]);
            index += strlen(&buffer[index]) + 1;
            nbAnswers -= 1;
        } else {
            msg_length = recvfrom(socket, &buffer, UDP_MAX_SIZE, 0, (struct sockaddr*) &serv_address, &sock_len);
            index = 0;
            if ( msg_length == -1 || address.sin_port != serv_address.sin_port || address.sin_addr.s_addr != serv_address.sin_addr.s_addr) {
                debug_print("%s", "No answers");
                fprintf(stdout, "FAIL\n");
                return 1;
            }
        }

    }

    return 0;
}

