
#include "error.h"
#include "node.h"
#include "util.h"
#include "system.h"
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define UINT_MAX_SIZE 6
#define SIZET_MAX_SIZE 2

error_code node_init(node_t *node, const char *ip, uint16_t port, size_t node_id){
	char* ip_copied = calloc(strlen(ip), sizeof(char));
	node->ip_size = strlen(ip);
	if (ip_copied == NULL){
		return ERR_NOMEM;
	}
	strncpy(ip_copied, ip, strlen(ip));
	node->ip = ip_copied;
	node->port = port;
	char buf[UINT_MAX_SIZE];
	char buf2[SIZET_MAX_SIZE];
	sprintf(buf, "%u", port);
	sprintf(buf2, "%zu", node_id);

	char line[strlen(ip) + 1 + strlen(buf) + 1 + strlen(buf2)];
	strcpy(&line[0], ip);
	line[strlen(ip)] = ' ';
	strcpy(&line[strlen(ip) + 1], buf);
	line[strlen(ip) + 1 + strlen(buf)] = ' ';
	strcpy(&line[strlen(ip) + 1 + strlen(buf) + 1], buf2);
	line[strlen(ip) + 1 + strlen(buf) + 1 + strlen(buf2)] = '\n';

	unsigned char* sha = calloc(SHA_DIGEST_LENGTH, sizeof(char));
	SHA1((unsigned char*) &line[0], strlen(ip) + 1 + strlen(buf) + 1 + strlen(buf2), sha);
	node->SHA = sha;

	return get_server_addr(ip, port,&node->address);
}

void node_end(node_t *node){
	free_const_ptr(node->ip);
	free(node->SHA);
	node->SHA = NULL;
}

int node_cmp_sha(const node_t *first, const node_t *second){
	const unsigned char* SHA_first = &first->SHA[0];
	const unsigned char* SHA_second = &second->SHA[0];
	for (size_t i = 0; i < SHA_DIGEST_LENGTH; ++i) {
		if (SHA_first[i] > SHA_second[i]){
			return 1;
		} else if (SHA_first[i] < SHA_second[i]){
			return -1;
		}
	}
	return 0;
}

#define FIRST 1
#define SECOND -1
#define EQUAL_SIZE 0

int node_cmp_server_addr(const node_t *first, const node_t *second){
	const char* IP_first = first->ip;
	const char* IP_second = second->ip;
	size_t min = first->ip_size > second->ip_size ? second->ip_size : first->ip_size;
	size_t longest_ip = first->ip_size > second->ip_size ? FIRST : SECOND;
	if (first->ip_size  ==  second->ip_size){
		longest_ip = 0;
	}
	for (size_t i = 0; i < min ; ++i) {
		if (IP_first[i] > IP_second[i]){
			return 1;
		} else if (IP_first[i] < IP_second[i]){
			return -1;
		}
	}
	if (longest_ip != 0){
		return longest_ip;
	}

	if (first->port > second->port){
		return 1;
	} else if(first->port < second->port){
		return -1;
	}
	return 0;
}