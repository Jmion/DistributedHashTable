//
// Created by Simon Wicky on 20.03.18.
//
#include "error.h"
#include "hashtable.h"
#include "client.h"


#define RETURN_MSG_LENGTH 4
struct sockaddr_in srv_addr;


error_code network_get(client_t client, pps_key_t key, pps_value_t *value){

    char out_msg = htonl(key);
    network_send(client,&out_msg);
};

error_code network_put(client_t client, pps_key_t key, pps_value_t value){


    return ERR_NONE;
};

error_code network_del(client_t client, pps_key_t key){
    return ERR_NONE;
};




error_code network_send(client_t client, char const* out_msg){
    struct sockaddr_in srv_addr = client.node.address;
    if (sendto(client.socket, &out_msg, sizeof(out_msg), 0,
               (struct sockaddr *) &srv_addr, sizeof(srv_addr)) == -1)
        die("sendto");
    return ERR_NONE;
}

error_code network_receive(){
    char in_msg[RETURN_MSG_LENGTH];
    ssize_t in_msg_len = rec(s, &in_msg, sizeof(in_msg), 0);
    if (in_msg_len == RETURN_MSG_LENGTH || in_msg_len == 0) { // Valid response.
        // Parse response with ntohl
        char response_msg[RETURN_MSG_LENGTH];
        (void)memset(&response_msg,0, sizeof(response_msg));
        for(int i = 0; i < RETURN_MSG_LENGTH; ++i){
            response_msg[i] = ntohl(in_msg[i]);
        }
        if(in_msg_len == 0){
            return ERR_NONE;
        }else if(in_msg_len == RETURN_MSG_LENGTH){

        }

    } else if (in_msg_len == -1) { // Timeout.
        M_EXIT_ERR(ERR_NETWORK,"timeout");
    } else { // Wrong message size.
        puts("Received invalid response.");
    }
}

error_code send(client_t client, const void* msg, size_t site){

}