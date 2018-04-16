#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

#define IP_SIZE 15


int main(void) {
	int socket = get_socket(0);

    char IP[IP_SIZE];
    int port = 0;


    fprintf(stdout, "IP Port? ");
    fflush(stdout);
	//TODO render this abble to get bad imput
    fscanf(stdin, "%s", &IP[0]);
    scanf("%d", &port);
    printf("Does nothing yet\n");
}

