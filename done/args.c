#include "args.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "error.h"
#include <stdio.h>

#define strtol(x) strtol(x, NULL, 10)
#define DEFAULT_N 3
#define DEFAULT_R 2
#define DEFAULT_W 2

args_t *parse_opt_args(size_t supported_args, char ***rem_argv){
	args_t* args = calloc(1, sizeof(args_t));
	args->N = DEFAULT_N;
	args->R = DEFAULT_R;
	args->W = DEFAULT_W;
	//discarding programm name
	++(*rem_argv);

	//looking for -n
	if (supported_args & TOTAL_SERVERS) {
		if (strcmp((*rem_argv)[0], "-n") == 0) {
			args->N = strtol((*rem_argv)[1]);
			if (args->N == 0) {
				return NULL;
			}
			debug_print("N is %zu",args->N);
			(*rem_argv) += 2;
		} else if (strcmp((*rem_argv)[0], "--") == 0) {
			debug_print("%s", "END OF OPTION");
			++(*rem_argv);
			return args;
		}
	}

	//looking for -r
	if (supported_args & GET_NEEDED) {
		if (strcmp((*rem_argv)[0], "-r") == 0){
			args->R = strtol((*rem_argv)[1]);
			if (args->R == 0) {
				return NULL;
			}
			debug_print("R is %zu",args->R);
			(*rem_argv) += 2;
		} else if (strcmp((*rem_argv)[0], "--") == 0) {
			++(*rem_argv);
			return args;		
		}

	}
	
	//looking for w
	if (supported_args & PUT_NEEDED) {
		if (strcmp((*rem_argv)[0], "-w") == 0) {
			args->W = strtol((*rem_argv)[1]);
			if (args->W == 0) {
				return NULL;
			}
			debug_print("W is %zu",args->W);
			(*rem_argv) += 2;
		} else if (strcmp((*rem_argv)[0], "--") == 0) {
			++(*rem_argv);
			return args;
		}
	}


	return args;
}