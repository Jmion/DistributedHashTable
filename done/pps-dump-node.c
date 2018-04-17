#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

#define IP_SIZE 15


int main(void) {
    char IP[IP_SIZE];
    int port = 0;


    fprintf(stdout, "IP Port? ");
    fflush(stdout);
	//TODO render this abble to get bad imput
    fscanf(stdin, "%s", &IP[0]);
    scanf("%d", &port);
    struct sockaddr_in address;
    get_server_addr(IP, port,&address);
    int socket = get_socket(1);
    char msg = '\0';
    if (sendto(socket, &msg, 1, 0, (struct sockaddr *) &address, sizeof(address)) == -1){
		debug_print("%s", "Sending failed.");
		fprintf(stdout, "FAIL\n");
		return -1;
	}

	//TODO handle answers
	printf("OK\n");
	return 0;
}

