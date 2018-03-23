//
// Created by Simon Wicky on 20.03.18.
//
#include "error.h"
#include "hashtable.h"
#include "client.h"


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
	return sendto(client.socket, msg, size, 0, (struct sockaddr *) &client.node.address, sizeof(client.node.address));
}

/**
 * Will return the size of the message reveiced. It will return 0 if it is properly exited. will return -1 in case of error
 * @param client
 * @param buffer
 * @param size
 * @return
 */
ssize_t receive_from_server(client_t client, void* buffer, size_t size){
	return recvfrom(client.socket, buffer, size, 0, NULL, NULL);
}
//*******END OF MODULARISATION*****

error_code network_get(client_t client, pps_key_t key, pps_value_t *value){
	//send get request
	if(send_server(client,&key, sizeof(key)) == -1){
		//TODO error message
		return ERR_NETWORK;
	}
	//receive response
	if (receive_from_server(client,value, sizeof(*value)) != sizeof(*value)){
		//TODO error message
		return ERR_NETWORK;
	}
	return ERR_NONE;
};

error_code network_put(client_t client, pps_key_t key, pps_value_t value){
	size_t size = sizeof(key)+ sizeof(value);
	unsigned char msg[size];
	//filling the message array.
	for(int i = 0 ; i < size-1; i++){
		msg[i+1] = htonl(value >> (8 * i));
	}
	msg[0] = htonl(key);

	//Sending the message
	if(-1 == send_server(client, &msg, size)){
		//TODO error print HERE
		return ERR_NETWORK;
	}

	//Receiving the reply/ sucessfull delivery
	//shoudl reveice size of value as the response.
	if(receive_from_server(client,NULL, 0) != 0){
		//TODO print message here
		return ERR_NETWORK;
	}

    return ERR_NONE;
};

error_code network_del(client_t client, pps_key_t key){
    return ERR_NONE;
};