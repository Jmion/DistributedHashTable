#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <string.h>
#include <stdio.h>

#define MSG_LENGTH 5




int main(void) {
	int socket = get_socket(0);

	bind_server(socket, PPS_DEFAULT_IP,PPS_DEFAULT_PORT);


	//creating hashtable. Should call construct hashtable when implemented.
	Htable_t hashtableTemp;
	(void)memset(&hashtableTemp, 0, HTABLE_SIZE);


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	while(1){
	struct sockaddr_in addr_cli;
	socklen_t addr_cli_len = sizeof(addr_cli);

	memset(&addr_cli,0,addr_cli_len);

	char in_msg[MSG_LENGTH];

	ssize_t msg_len = recvfrom(socket, &in_msg,sizeof(in_msg), 0, (struct sockaddr *)&addr_cli, &addr_cli_len);
	if ( msg_len == -1) {
		M_EXIT(ERR_NETWORK,"Message received in not of appropriate length.");
	} else  if (msg_len == 5){ //put request
		char content[MSG_LENGTH];
		for (int i = 0; i < MSG_LENGTH; ++i) {
			content[i] = ntohl(in_msg[i]);
		}
		error_code e = add_Htable_value(hashtableTemp,content[0],content[1]);
		M_EXIT_IF_ERR(e,"Issue adding key to hasahtable");

	 } else { //get request
	 	pps_key_t key = ntohl(in_msg[0]);
		get_Htable_value(hashtableTemp, key);
	 }
	}
#pragma clang diagnostic pop

	return 0;
}
