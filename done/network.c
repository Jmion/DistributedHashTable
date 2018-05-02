//
// Created by Simon Wicky on 20.03.18.
//
#include "error.h"
#include "hashtable.h"
#include "client.h"
#include <stdio.h>
#include "config.h"


#define RETURN_MSG_LENGTH 4
#define PUT_REQUEST 1
#define GET_REQUEST 0
struct sockaddr_in srv_addr;


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
	return recv(socket, buffer, buffer_size, 0);
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
	size_t nbValidAnswers = 0;
	size_t nbResponse = 0;
	ssize_t length = -1;
	Htable_t local_htable = construct_Htable(HTABLE_SIZE);
	size_t max_value = 0;

	//the while loop must loop on client.args->N
	//nbResponseRequired is client.args->W if put request
	//nbValidAnswersRequired is client.args->R if get request. Check the equality of the value

	while(index < client.node_list->size){
		ssize_t msg_length = send_and_get(client.socket, &client.node_list->nodes[index].address, msg, msg_size, buffer, buffer_size);
		if(msg_length != -1){
			nbResponse += 1;
			pps_key_t tempKey = buffer;
			pps_value_t responsePoint = get_Htable_value(local_htable, tempKey);
			if(responsePoint == NULL){ //first time the receive this value
				add_Htable_value(local_htable, tempKey, "\x01"); //initialising count to 1
				max_value = max_value > 1 ? max_value : 1;
				if(1 >= client.args->R){
					return msg_length;
				}
			}else{
				char nbRes = responsePoint[0];
				++nbRes;
				add_Htable_value(local_htable,tempKey,(pps_value_t)(&nbRes)); // increasing the count of the know value
				max_value = max_value > nbRes ? max_value : nbRes;
				if(nbRes >= client.args->R){
					return msg_length;
				}
			}
			//valid if not '\0'
			if(msg_length != 1 || ((char*) buffer)[0] != '\0'){
				nbValidAnswers += 1;
				length = msg_length;
			}
		}
		++index;
	}

	if ((nbResponse < client.args->W && putRequest) ) {
		debug_print("%s %zu %s %zu", "Missing response from server, only got ", nbResponse, "response(s), needing ", client.args->W);
		return -1;
	}
	return -1;
}

//*******END NEW MODULARISATION***


#define nbValidAnswersNeeded 1

error_code network_get(client_t client, pps_key_t key, pps_value_t* value){

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