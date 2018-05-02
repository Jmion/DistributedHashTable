#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

#define IP_SIZE 15
#define UDP_MAX_SIZE 65507 //9217


int main(void) {
	int socket = get_socket(0);

    char IP[IP_SIZE+1];
    int port = 0;


    fprintf(stdout, "IP Port? ");
    fflush(stdout);
    fscanf(stdin, "%s", &IP[0]);
    fscanf(stdin,"%d", &port);

	if (bind_server(socket, IP, port)) {
		debug_print("%s\n", "Server failed to bind to socket");
	}

	Htable_t htable = construct_Htable(HTABLE_SIZE);

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

		} else if (msg_len == 0){ //check request
			debug_print("%s", "Check request OK");
			if(sendto(socket, NULL, 0, 0, (struct sockaddr *) &addr_cli, addr_cli_len) == -1){
				debug_print("%s\n","Error sending acknoledgement of check request.");
			}

			
		} else if (msg_len == 1 && in_msg[0] == '\0'){//dump request
			debug_print("%s", "Dump request OK");
			char msg[UDP_MAX_SIZE];
			memset(msg,0,UDP_MAX_SIZE);
			size_t msg_len = 0;
			unsigned int size_send = htonl(htable->nbElements);
			memcpy(&msg[0],&size_send,sizeof(size_send));
			msg_len += sizeof(unsigned int);



			for (int i = 0; i < htable->size; ++i) {
				bucket_t* bucket = &htable->content[i];
				while(bucket != NULL && bucket->pair.key != NULL){

					if (msg_len + strlen(bucket->pair.key) + strlen(bucket->pair.value) + 2 > UDP_MAX_SIZE) {
						if(sendto(socket, &msg, msg_len, 0, (struct sockaddr *) &addr_cli, addr_cli_len)== -1){
							debug_print("%s","Could not send ");
						}
						debug_print("%s","New message");
						msg_len = 0;
						memset(msg,0,UDP_MAX_SIZE);

					}
					debug_print("%s", "Copying key-value");
					strncpy(&msg[msg_len], bucket->pair.key, strlen(bucket->pair.key));
					msg_len += strlen(bucket->pair.key);
					msg[msg_len] = '\0';
					msg_len += 1;
					strncpy(&msg[msg_len], bucket->pair.value, strlen(bucket->pair.value));
					msg_len += strlen(bucket->pair.value);
					msg[msg_len] = '\0';
					msg_len += 1;
					bucket = bucket->next;

				}
			}

			if(sendto(socket, &msg, msg_len, 0, (struct sockaddr *) &addr_cli, addr_cli_len)== -1){
				debug_print("%s","Could not send ");
			}


		} else if (memchr(&in_msg,'\0', msg_len) != NULL) { //put request
			debug_print("%s", "Put request OK");

			size_t key_len = strlen(in_msg); // locates the \0 in the string
			size_t value_len = msg_len - key_len - 1;
			char key[key_len+1]; //extra space of '\0' at the end
			char value [value_len + 1];
			strncpy(key,in_msg,key_len);
			strncpy(value, &in_msg[key_len+1], value_len);
			key[key_len] = '\0';
			value[value_len] = '\0';

			


			error_code e = add_Htable_value(htable, key, value);
			M_EXIT_IF_ERR(e, "Issue adding key to hasahtable");

			if(sendto(socket, &key, 0, 0, (struct sockaddr *) &addr_cli, addr_cli_len) == -1){
				debug_print("%s\n","Error sending acknoledgement of put request.");
			}
		} else { //get request
			debug_print("%s", "get request OK");
			char key[msg_len+1];
			strncpy(key, in_msg, msg_len);
			key[msg_len] = '\0';

			pps_value_t value = get_Htable_value(htable, key);
			size_t length;
			if (value == NULL){
				value = "\0";
				length = 1;
				debug_print("%s","NULL value");
			} else {
				length = strlen(value);
			}

				if(sendto(socket, value, length, 0, (struct sockaddr *) &addr_cli, addr_cli_len) == -1){
					debug_print("%s","Error responding to get request. Sending value failed");
				}
		}
	}
#pragma clang diagnostic pop
	return 0;
}

