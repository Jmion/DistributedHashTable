//
// Created by Simon Wicky on 20.03.18.
//
#include "error.h"
#include "hashtable.h"
#include "client.h"
#include <stdio.h>
#include "config.h"


#define RETURN_MSG_LENGTH 4
struct sockaddr_in srv_addr;

//MODULARISATION


/**
 *
 * @param client
 * @param msg
 * @param size
 * @return -1 in case of error
 */

//DEPRECATED METHOD
ssize_t send_server(client_t client, const void* msg, size_t size){
	ssize_t flag = 0;
	for (int i = 0; i < client.node_list->size; ++i) {
		if(sendto(client.socket, msg, size, 0, (struct sockaddr *) &client.node_list->nodes[i].address, sizeof(client.node_list->nodes[i].address)) == -1){
			flag = -1;
		}
	}
	return flag;
}

/**
 * Will return the size of the message reveiced. It will return 0 if it is properly exited. will return -1 in case of error
 * @param client client to receive from
 * @param buffer location where the data received from the server will be stored
 * @param size size of the buffer
 * @param nbToReceiveFrom number of servers that need to respond for valide response
 * @param nbResponse number of servers that responded
 * @return
 */

//DEPRECATED METHOD
ssize_t receive_from_server(client_t client, void* buffer, size_t size, size_t nbValidAnswersRequired, size_t* nbValidAnswersGot, size_t* length){
	ssize_t nbResponse = 0;
	size_t validAnswers = 0;
	size_t index = 0;
	while(index < client.node_list->size && validAnswers < nbValidAnswersRequired){
		printf("Recieving\n");
		ssize_t msg_length = 0;
		if((msg_length = recv(client.socket, buffer, size, 0)) != -1 ){
			printf("%zu\n",msg_length);
			if(msg_length != 1 || ((char*) buffer)[0] != '\0'){
				validAnswers += 1;
				if (length != NULL) {
					*length = msg_length;
				}
			} else {
				printf("Unvalid answer\n");
			}
			nbResponse += 1;
		}
		++index;
	}
	if (nbValidAnswersGot != NULL) { // possibility to pass a pointer null
		*nbValidAnswersGot = validAnswers;
	}
	return nbResponse;
}
//*******END OF MODULARISATION*****

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

ssize_t network_comm(client_t client, const void* msg, size_t msg_size, void*buffer, size_t buffer_size, size_t nbValidAnswersRequired, size_t nbResponseRequired){
	size_t index = 0;
	size_t nbValidAnswers = 0;
	size_t nbResponse = 0;
	ssize_t length = -1;

	while(index < client.node_list->size && nbValidAnswers < nbValidAnswersRequired){
		ssize_t msg_length = send_and_get(client.socket, &client.node_list->nodes[index].address, msg, msg_size, buffer, buffer_size);
		if(msg_length != -1){
			nbResponse += 1;

			//valid if not '\0'
			if(msg_length != 1 || ((char*) buffer)[0] != '\0'){
				nbValidAnswers += 1;
				length = msg_length;
			}
		}



		++index;
	}

	if (nbResponse < nbResponseRequired) {
		debug_print("%s %zu %s", "Missing response from server, only got ", nbResponse, "response(s)");
		return -1;
	}

	if (nbValidAnswers < nbValidAnswersRequired) {
		debug_print("%s %zu %s", "Not enough valid answers, only got ", nbValidAnswers, "valid answer(s)");
		return -1;
	}
	return length;
}





//*******END NEW MODULARISATION***


#define nbValidAnswersNeeded 1

error_code network_get(client_t client, pps_key_t key, pps_value_t* value){

	//send get request
	char key_msg[strlen(key)];
	strncpy(key_msg,key,strlen(key));



	// if(send_server(client,key_msg, strlen(key)) == -1){
	// 	debug_print("%s\n", "NETWORK_GET : Sending key to server failed");
	// 	return ERR_NETWORK;
	// }

	//receive response
	
	char value_msg[MAX_MSG_ELEM_SIZE];


	// size_t nbValidAnswersGot = 0;
	// size_t msg_length = 0;
	// ssize_t nbResponse = receive_from_server(client, value_msg, MAX_MSG_ELEM_SIZE ,nbValidAnswersNeeded, &nbValidAnswersGot, &msg_length);
	// printf("%zu\n", nbValidAnswersGot);
	// if (nbResponse == 0){
	// 	debug_print("%s", "NETWORK_GET : Receiving reply from server failed");
	// 	return ERR_NETWORK;
	// }
	// if(nbValidAnswersGot == 0){
	// 	debug_print("%s", "NO VALID KEY : key doesn't exist");
	// 	return ERR_NOT_FOUND;
	// }
	ssize_t msg_length = network_comm(client, key_msg, strlen(key), value_msg, MAX_MSG_ELEM_SIZE, nbValidAnswersNeeded, 0);


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




	//Sending the message
	// if(-1 == send_server(client, msg, size+1)){
	// 	debug_print("%s\n", "NETWORK_PUT : Sending failed.");
	// 	return ERR_NETWORK;
	// }


	//shoudl receive size of value as the response.
	char* in_msg;

	ssize_t msg_length = network_comm(client, msg, size +1, in_msg, 1, client.node_list->size, client.node_list->size);

	if (msg_length == -1) {
		debug_print("%s", "NETWORK_COMM : Something failed");
		return ERR_NETWORK;
	}

	// ssize_t nbResponse = receive_from_server(client,in_msg, 1,client.node_list->size, NULL, NULL);
	// if ( nbResponse != client.node_list->size) {
	// 	debug_print("%s %zu %s", "Missing response from server, only got ", nbResponse, "response(s)");
	// 	return ERR_NETWORK;
	// }
    return ERR_NONE;
};

error_code network_del(client_t client, pps_key_t key){
    return ERR_NONE;
};