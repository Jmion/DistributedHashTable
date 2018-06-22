#include "util.h"
#include "error.h"
#include "node.h"
#include "util.h"
#include "system.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define IP_MAX_SIZE 15
#define UINT_MAX_SIZE 5
#define SIZET_MAX_SIZE 20

error_code node_init(node_t *node, const char *ip, uint16_t port, size_t node_id) {
	node->ip = strdup(ip);
	node->ip_size = strlen(ip);
	if (node->ip == NULL) {
		return ERR_NOMEM;
	}
	node->port = port;
	char line[IP_MAX_SIZE + 1 + UINT_MAX_SIZE + 1 + SIZET_MAX_SIZE];
	memset(line, 0, IP_MAX_SIZE + 1 + UINT_MAX_SIZE + 1 + SIZET_MAX_SIZE);
	sprintf(line, "%s %u %zu", ip, port, node_id);

	node->SHA = calloc(SHA_DIGEST_LENGTH, sizeof(char));
	if (node->SHA == NULL) {
		return ERR_NOMEM;
	}
	SHA1((unsigned char*) &line[0], strlen(line), node->SHA);

	return get_server_addr(ip, port, &node->address);
}

void node_end(node_t *node) {
	free_const_ptr(node->ip);
	free(node->SHA);
	node->SHA = NULL;
	node->ip = NULL;
}

int node_cmp_sha(const node_t *first, const node_t *second) {
	const unsigned char* SHA_first = &first->SHA[0];
	const unsigned char* SHA_second = &second->SHA[0];
	for (size_t i = 0; i < SHA_DIGEST_LENGTH; ++i) {
		if (SHA_first[i] > SHA_second[i]) {
			return 1;
		} else if (SHA_first[i] < SHA_second[i]) {
			return -1;
		}
	}
	return 0;
}

#define FIRST 1
#define SECOND -1
#define EQUAL_SIZE 0

int node_cmp_server_addr(const node_t *first, const node_t *second) {
	const char* IP_first = first->ip;
	const char* IP_second = second->ip;
	size_t min = first->ip_size > second->ip_size ? second->ip_size : first->ip_size;
	size_t longest_ip = first->ip_size > second->ip_size ? FIRST : SECOND;
	if (first->ip_size  ==  second->ip_size) {
		longest_ip = EQUAL_SIZE;
	}
	for (size_t i = 0; i < min ; ++i) {
		if (IP_first[i] > IP_second[i]) {
			return 1;
		} else if (IP_first[i] < IP_second[i]) {
			return -1;
		}
	}
	if (longest_ip != EQUAL_SIZE) {
		return longest_ip;
	}

	if (first->port > second->port) {
		return 1;
	} else if (first->port < second->port) {
		return -1;
	}
	return node_cmp_sha(first, second);
}