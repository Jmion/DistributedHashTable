#include "system.h"
#include "config.h"
#include <string.h>
#include <stdio.h>

#define MSG_LENGTH 5




int main(void) {
	int socket = get_socket(0);
	error_code srv_err = bind_server(socket, PPS_DEFAULT_IP,PPS_DEFAULT_PORT);


	struct sockaddr_in addr_cli;
	socklen_t addr_cli_len = sizeof(addr_cli);

	memset(&addr_cli,0,addr_cli_len);

	char in_msg[MSG_LENGTH];

	ssize_t msg_len = recvfrom(socket, &in_msg,sizeof(in_msg), 0, (struct sockaddr *)&addr_cli, &addr_cli_len);
	if ( msg_len == -1) {
		//Erreur
	} else  if (msg_len == 5){ //put request
		char content[MSG_LENGTH];
		for (int i = 0; i < MSG_LENGTH; ++i) {
			content[i] = ntohl(in_msg[i]);
		}


		
	 } else { //get request
	 	char key = ntohl(in_msg[0]);

	 }


	return 0;
}