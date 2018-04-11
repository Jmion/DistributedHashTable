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

	if (bind_server(socket, IP, port)) {
		debug_print("%s\n", "Server failed to bind to socket");
	}

	Htable_t htable = construct_Htable(HTABLE_SIZE);
	(void) memset(&htable, 0, sizeof(bucket_t) * HTABLE_SIZE); //Htable is formed of 32 bit ints. Therefor we


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	while (1) {
		struct sockaddr_in addr_cli;
		socklen_t addr_cli_len = sizeof(addr_cli);
		memset(&addr_cli, 0, addr_cli_len);

		char in_msg[MAX_MSG_SIZE];

		ssize_t msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0, (struct sockaddr *) &addr_cli, &addr_cli_len);

		if (msg_len == -1) {
			M_EXIT_IF_ERR(ERR_NETWORK, "Message received in not of appropriate length.");
		} else if (memchr(&in_msg,'\0', sizeof(in_msg)) != NULL) { //put request
			printf("Put request OK\n");

			size_t locationOfKVSeperator = strlen(in_msg); // locates the \o in the string
			char key[locationOfKVSeperator+1]; //extra space of '\0' at the end
			char value [msg_len-locationOfKVSeperator];
			strncpy(key,in_msg,locationOfKVSeperator);
			strncpy(value, &in_msg[locationOfKVSeperator+1], msg_len-locationOfKVSeperator-1);
			key[locationOfKVSeperator + 1] = '\0';
			value[msg_len - locationOfKVSeperator] = '\0';



			error_code e = add_Htable_value(htable, key, value);
			M_EXIT_IF_ERR(e, "Issue adding key to hasahtable");

			if(sendto(socket, &key, 0, 0, (struct sockaddr *) &addr_cli, addr_cli_len) == -1){
				debug_print("%s\n","Error sending acknoledgement of put request.");
			}
		} else { //get request
			printf("get request OK\n");
			char key[msg_len+1];
			strncpy(key, in_msg, msg_len);
			key[msg_len + 1] = '\0';

			pps_value_t value = get_Htable_value(htable, key);

			if(sendto(socket, &value, sizeof(value), 0, (struct sockaddr *) &addr_cli, addr_cli_len) == -1){
				debug_print("%s\n","Error responding to get request. Sending value failed");
			}
		}
	}
#pragma clang diagnostic pop

	return 0;
}

