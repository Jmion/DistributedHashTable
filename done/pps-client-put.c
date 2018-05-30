#include "client.h"
#include "node.h"
#include "network.h"
#include <stdio.h>
#include "node_list.h"
#include "config.h"
#include "error.h"
#include "args.h"
#include <time.h>

#define OUT_NAME "measure_put.txt"


static void print_time(const struct timespec*, const struct timespec*, char*);


int main(int argc, char *argv[]) {
    client_init_args_t init_client;
    client_t cl;
    init_client.client = &cl;
    init_client.argv = &argv;
    init_client.argc = argc;
    init_client.nodes_list = ring_alloc();
    ring_init(init_client.nodes_list);
    init_client.argsRequired = TOTAL_SERVERS | PUT_NEEDED;
    char** first = &argv[0];
    error_code errCode = client_init(init_client);
    M_EXIT_IF_ERR(errCode, "Error initializing client");
    client_t* client = init_client.client;
    size_t nbArgsLeft = argc - (&argv[0] - first);

    if ( nbArgsLeft != 2) {
        debug_print("Wrong number of arguments, %zu arguments", nbArgsLeft);
        printf("FAIL\n");
        client_end(client);
        return 1;
    }


    struct timespec time_start, time_end;
    clock_gettime(CLOCK_MONOTONIC, &time_start);

    error_code error = network_put(*client, argv[0], argv[1]);

    clock_gettime(CLOCK_MONOTONIC, &time_end);
    print_time(&time_start, &time_end, OUT_NAME);

    if (error != 0) {
        printf("FAIL\n");
    } else {
        printf("OK\n");
    }


    client_end(client);
    return 0;
}


static void print_time(const struct timespec* p_time_start,
const struct timespec* p_time_end, char* fname) {
FILE* out = fopen(fname,"a");
if (out == NULL){
    return;
}
long nsec = p_time_end->tv_nsec - p_time_start->tv_nsec;
while (nsec < 0) nsec += 1000000000;
fprintf(out, "%ld%09ld\n", p_time_end->tv_sec - p_time_start->tv_sec, nsec);
fclose(out);
}