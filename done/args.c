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

	//looking for -n
	if (supported_args & TOTAL_SERVERS) {
		if ((*rem_argv)[0] != NULL && (*rem_argv)[1] != NULL) {
			if (strcmp((*rem_argv)[0], "-n") == 0) {
				errno = 0;
				args->N = strtol((*rem_argv)[1]);
				if (errno != 0 || args->N <= 0) {
					debug_print("%s", "Couldn't convert option for N, or N smaller than 0");
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
	}

	//looking for -r
	if (supported_args & GET_NEEDED) {
		if ((*rem_argv)[0] != NULL && (*rem_argv)[1] != NULL) {
			if (strcmp((*rem_argv)[0], "-r") == 0){
				errno = 0;
				args->R = strtol((*rem_argv)[1]);
				if (errno != 0 || args->R <= 0) {
					debug_print("%s", "Couldn't convert option for R, or R smaller than 0");
					return NULL;
				}
				debug_print("R is %zu",args->R);
				(*rem_argv) += 2;
			} else if (strcmp((*rem_argv)[0], "--") == 0) {
				debug_print("%s", "END OF OPTION");
				++(*rem_argv);
				return args;		
			}
		}

	}
	
	//looking for w
	if (supported_args & PUT_NEEDED) {
		if ((*rem_argv)[0] != NULL && (*rem_argv)[1] != NULL) {
			if (strcmp((*rem_argv)[0], "-w") == 0) {
				errno = 0;
				args->W = strtol((*rem_argv)[1]);
				if (errno != 0) {
					debug_print("%s", "Couldn't convert option for W, or W smaller than 0");
					return NULL;
				}
				debug_print("W is %zu",args->W);
				(*rem_argv) += 2;
			} else if (strcmp((*rem_argv)[0], "--") == 0) {
				debug_print("%s", "END OF OPTION");		
				++(*rem_argv);
				return args;
			}
		}
	}
	if (supported_args & (TOTAL_SERVERS | GET_NEEDED | PUT_NEEDED)) {
		if ((*rem_argv)[0] != NULL) {
			if (strcmp((*rem_argv)[0], "--") == 0) {
				++(*rem_argv);
			}
		}
	}


	return args;
}