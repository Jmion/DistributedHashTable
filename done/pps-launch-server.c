#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

#define MSG_LENGTH 5
#define IP_SIZE 15


int main(void) {
	int socket = get_socket(0);

    char IP[IP_SIZE + 1];
    int port = 0;


    fprintf(stdout, "IP Port? ");
    fflush(stdout);
    fscanf(stdin, "%s", &IP[0]);
    scanf("%d", &port);

	if (bind_server(socket, IP, port)) {
		debug_print("%s\n", "Server failed to bind to socket");
	}



	//creating hashtable. Should call construct hashtable when implemented.
	Htable_t hashtableTemp;
	(void) memset(&hashtableTemp, 0, sizeof(bucket_t) * HTABLE_SIZE); //Htable is formed of 32 bit ints. Therefor we


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	while (1) {
		struct sockaddr_in addr_cli;
		socklen_t addr_cli_len = sizeof(addr_cli);
		memset(&addr_cli, 0, addr_cli_len);

		char in_msg[MSG_LENGTH];

		ssize_t msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0, (struct sockaddr *) &addr_cli, &addr_cli_len);

		if (msg_len == -1) {
			M_EXIT(ERR_NETWORK, "Message received in not of appropriate length.");
		} else if (msg_len == 5) { //put request
			printf("Put request OK\n");
			uint32_t netValue;
			memcpy(&netValue, &in_msg[1], sizeof(netValue));
			pps_value_t value = ntohl(netValue);
			pps_key_t key = in_msg[0];

			error_code e = add_Htable_value(hashtableTemp, key, value);
			M_EXIT_IF_ERR(e, "Issue adding key to hasahtable");

			if(sendto(socket, &key, 0, 0, (struct sockaddr *) &addr_cli, addr_cli_len) == -1){
				debug_print("%s\n","Error sending acknoledgement of put request.");
			}
		} else { //get request
			printf("get request OK\n");
			pps_key_t key = in_msg[0];
			pps_value_t value = get_Htable_value(hashtableTemp, key);

			uint32_t netValue = htonl(value);
			if(sendto(socket, &netValue, sizeof(netValue), 0, (struct sockaddr *) &addr_cli, addr_cli_len)){
				debug_print("%s\n","Error responding to get request. Sending value failed");
			}
		}
	}
#pragma clang diagnostic pop

	return 0;
}

