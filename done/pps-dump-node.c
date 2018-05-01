#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

#define IP_SIZE 15
#define UDP_MAX_SIZE 65507


int main(void) {
    char IP[IP_SIZE+1];
    int port = 0;


    fscanf(stdin, "%s", &IP[0]);
    scanf("%d", &port);
    struct sockaddr_in address;
    get_server_addr(IP, port,&address);
    int socket = get_socket(1);
    char msg = '\0';
    if (sendto(socket, &msg, 1, 0, (struct sockaddr *) &address, sizeof(address)) == -1){
		debug_print("%s", "Sending failed.");
		fprintf(stdout, "FAIL\n");
		return 1;
	}

	unsigned int nbAnswers = 1;
    char buffer[UDP_MAX_SIZE];
    size_t msg_length;
    msg_length = recv(socket, &buffer, UDP_MAX_SIZE, 0);
    if( msg_length== -1){
        debug_print("%s", "No answers");
        fprintf(stdout, "FAIL\n");
        return 1;
    }

    memcpy(&nbAnswers, buffer, sizeof(unsigned int));
    nbAnswers = ntohl(nbAnswers);
    size_t index = sizeof(unsigned int);

    while(nbAnswers > 0){
        if (index < msg_length) {
            printf("%s", &buffer[index]);
            index += strlen(&buffer[index]) + 1;
            printf(" = %s\n",&buffer[index]);
            index += strlen(&buffer[index]) + 1;
            nbAnswers -= 1;
        } else {
            msg_length = recv(socket, &buffer, UDP_MAX_SIZE, 0);
            index = 0;
            if( msg_length== -1){
                debug_print("%s", "No answers");
                fprintf(stdout, "FAIL\n");
            return 1;
            }
        }

    }

	return 0;
}

