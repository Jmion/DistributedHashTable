#include "args.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "error.h"

#define strtol(x) strtol(x, NULL, 10)
#define DEFAULT_N 3
#define DEFAULT_R 2
#define DEFAULT_W 2

args_t *parse_opt_args(size_t supported_args, char ***rem_argv){
	debug_print("%s", "Parse opt called");
	char** argv = *rem_argv;
	args_t* args = calloc(1, sizeof(args_t));
	args->N = DEFAULT_N;
	args->R = DEFAULT_R;
	args->W = DEFAULT_W;
	//discarding programm name
	++argv;

	//looking for -n
	if (supported_args & TOTAL_SERVERS) {
		if (strcmp(argv[0], "-n") == 0) {
			debug_print("%s", "Found N option");
			args->N = strtol(argv[1]);
			if (args->N == 0) {
				return NULL;
			}
			argv += 2;
		} else if (strcmp(argv[0], "--")) {
			return args;
		}
	}

	//looking for -r
	if (supported_args & GET_NEEDED) {
		if (strcmp(argv[0], "-r") == 0){
			debug_print("%s", "Found R option");
			args->R = strtol(argv[1]);
			if (args->R == 0) {
				return NULL;
			}
			argv += 2;
		} else if (strcmp(argv[0], "--")) {
			return args;		
		}

	}
	debug_print("%s", "Programm is here");
	//looking for w
	if (supported_args & PUT_NEEDED) {
		if (strcmp(argv[0], "-w") == 0) {
			debug_print("%s", "Found W option");
			args->W = strtol(argv[1]);
			if (args->W == 0) {
				return NULL;
			}
			argv += 2;
		} else if (strcmp(argv[0], "--")) {
			return args;
		}
	}



	return args;
}