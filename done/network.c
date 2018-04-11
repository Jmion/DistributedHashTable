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
 * @param client
 * @param buffer
 * @param size
 * @return
 */
ssize_t receive_from_server(client_t client, void* buffer, size_t size,size_t nbToReceiveFrom, size_t* nbResponse){
	ssize_t total = 0;
	ssize_t nbRemainingToReceive = nbToReceiveFrom;
	for(size_t i = 0; i < client.node_list->size && nbToReceiveFrom >0; ++i){
		ssize_t temp = 0;
		if((temp = recv(client.socket, buffer, size, 0)) != -1 ){
			--nbRemainingToReceive;
			if (nbResponse != NULL) {
				*nbResponse += 1;
			}
			total = temp;
		}
	}
	return total ;// min(client.node_list->size,nbToReceiveFrom);
}
//*******END OF MODULARISATION*****
#define nbToReceiveFrom 1

error_code network_get(client_t client, pps_key_t key, pps_value_t* value){

	//send get request
	char key_msg[strlen(key)];
	strncpy(key_msg,key,strlen(key));



	if(send_server(client,key_msg, strlen(key)) == -1){
		debug_print("%s\n", "NETWORK_GET : Sending key to server failed");
		return ERR_NETWORK;
	}

	//receive response
	
	char value_msg[MAX_MSG_ELEM_SIZE];
	ssize_t msg_length = receive_from_server(client, value_msg, MAX_MSG_ELEM_SIZE ,nbToReceiveFrom, NULL);
	
	if (msg_length == -1){
		debug_print("%s\n", "NETWORK_GET : Receiving reply from server failed");
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
	if(-1 == send_server(client, msg, size+1)){
		debug_print("%s\n", "NETWORK_PUT : Sending failed.");
		return ERR_NETWORK;
	}

	//shoudl receive size of value as the response.
	char* in_msg;
	size_t nbResponse = 0;
	if(receive_from_server(client,in_msg, 1,client.node_list->size, &nbResponse) != 0){
		debug_print("%s\n", "NETWORK_PUT : receiving response unsuccessful");
		return ERR_NETWORK;
	}
	if (nbResponse != client.node_list->size) {
		debug_print("%s %zu %s", "Missing response from server, only got ", nbResponse, "response(s)");
		return ERR_NETWORK;
	}
    return ERR_NONE;
};

error_code network_del(client_t client, pps_key_t key){
    return ERR_NONE;
};