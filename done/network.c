#include "error.h"
#include "hashtable.h"
#include "client.h"
#include <stdio.h>
#include "config.h"


#define PUT_REQUEST 1
#define GET_REQUEST 0


//*******NEW MODULARISATION*******


/**
 * @brief      Send a msg to a server and get the answer
 *
 * @param[in]  int          The socket
 * @param      address      The address
 * @param[in]  msg          The message
 * @param[in]  msg_size     The message size
 * @param      buffer       The buffer
 * @param[in]  buffer_size  The buffer size
 *
 * @return     the length of the answer or -1 if something failed
 */
ssize_t send_and_get(int socket, struct sockaddr_in* address, const void* msg, size_t msg_size, void* buffer, size_t buffer_size){
	if (sendto(socket, msg, msg_size, 0, (struct sockaddr *) address, sizeof(*address)) == -1){
		debug_print("%s", "Sending failed.");
		return -1;
	}
	struct sockaddr_in serv_address;
	socklen_t serv_size = 0;
	ssize_t length = recvfrom(socket, buffer, buffer_size, 0,(struct sockaddr *) &serv_address, &serv_size);
	if (address->sin_addr.s_addr != serv_address.sin_addr.s_addr || address->sin_port != serv_address.sin_port){
		debug_print("%d, %d", address->sin_port, serv_address.sin_port);
		debug_print("%s", "Different server address");
		return -1;
	}

	return length;
}


/**
 * @brief      Handles the communication with all nodes of a client
 *
 * @param[in]  client                  The client
 * @param[in]  msg                     The message to send
 * @param[in]  msg_size                The message size
 * @param      buffer                  The buffer
 * @param[in]  buffer_size             The buffer size
 * @param[in]  nbValidAnswersRequired  The number of valid answers required
 * @param[in]  nbResponseRequired      The number of response required
 *
 * @return     the length of the last answer, or -1 if something failed
 */
ssize_t network_comm(client_t client, const void* msg, size_t msg_size, void*buffer, size_t buffer_size, int putRequest){
	size_t index = 0;
	size_t nbResponse = 0;
	Htable_t local_htable = construct_Htable(HTABLE_SIZE);
	size_t max_value = 0;

	while(index < client.args->N){
		ssize_t msg_length = send_and_get(client.socket, &client.node_list->nodes[index].address, msg, msg_size, buffer, buffer_size);
		if(msg_length != -1){
			nbResponse += 1;
			if(msg_length != 0 && ((char*) buffer)[0] != '\0') {
				pps_key_t tempKey = buffer;
				pps_value_t responsePoint = get_Htable_value(local_htable, tempKey);
				if (responsePoint == NULL) { //first time the receive this value
					error_code err = add_Htable_value(local_htable, tempKey, "\x01"); //initialising count to 1
					if (err != ERR_NONE){
						delete_Htable_and_content(&local_htable);
						return -1;
					}
					max_value = max_value > 1 ? max_value : 1;
					if (1 >= client.args->R) {
						delete_Htable_and_content(&local_htable);
						return msg_length;
					}
				} else {
					char nbRes = responsePoint[0];
					++nbRes;
					add_Htable_value(local_htable, tempKey,
					                 (pps_value_t) (&nbRes)); // increasing the count of the know value
					max_value = max_value > nbRes ? max_value : nbRes;
					if (nbRes >= client.args->R) {
						delete_Htable_and_content(&local_htable);
						return msg_length;
					}
				}
			}
		}
		++index;
	}
	delete_Htable_and_content(&local_htable);
	if ((nbResponse < client.args->W && putRequest) ) {
		debug_print("%s %zu %s %zu", "Missing response from server, only got ", nbResponse, "response(s), needing ", client.args->W);
		return -1;
	}
	if(nbResponse >= client.args->W && putRequest){
		return 1;
	}

	debug_print("%s", "GET didn't get enough identical responses");
	return -1;
}

//*******END NEW MODULARISATION***


#define nbValidAnswersNeeded 1

error_code network_get(client_t client, pps_key_t key, pps_value_t* value){
	M_EXIT_IF_TOO_LONG(key, MAX_MSG_ELEM_SIZE, "Key too long");

	debug_print("N is %zu", client.args->N);
	debug_print("R is %zu", client.args->R);
	//send get request
	char key_msg[strlen(key)];
	strncpy(key_msg,key,strlen(key));
	char value_msg[MAX_MSG_ELEM_SIZE];
	ssize_t msg_length = network_comm(client, key_msg, strlen(key), value_msg, MAX_MSG_ELEM_SIZE,GET_REQUEST);

	if (msg_length == -1) {
		debug_print("%s", "NETWORK_COMM : Something failed");
		return ERR_NETWORK;
	}
	 
	value_msg[msg_length] = '\0';
	char value_get[msg_length + 1];
	strncpy(value_get, value_msg, msg_length + 1);
	*value = value_get;
	return ERR_NONE;
}

error_code network_put(client_t client, pps_key_t key, pps_value_t value){
	debug_print("N is %zu", client.args->N);
	debug_print("W is %zu", client.args->W);
	M_EXIT_IF_TOO_LONG(key, MAX_MSG_ELEM_SIZE, "Key too long");
	M_EXIT_IF_TOO_LONG(value, MAX_MSG_ELEM_SIZE, "Value too long");	
	size_t size = strlen(key)+ strlen(value);
	char msg[size + 1];

	strncpy(&msg[0], key, strlen(key));
	msg[strlen(key)] = '\0';
	strncpy(&msg[strlen(key) + 1], value, strlen(value));

	char in_msg[1];

	ssize_t msg_length = network_comm(client, msg, size +1, in_msg, 1,PUT_REQUEST);

	if (msg_length == -1) {
		debug_print("%s", "NETWORK_COMM : Something failed");
		return ERR_NETWORK;
	}
    return ERR_NONE;
};

error_code network_del(client_t client, pps_key_t key){
    return ERR_NONE;
};