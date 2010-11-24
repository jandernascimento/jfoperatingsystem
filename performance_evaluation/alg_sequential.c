#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "alg_principal.h"
#include "call_sequential.h"

void call_sort(int parallelism, int *vector, int size, char *arg) {
	void *args_algo;
	arguments_t args;

	args_algo = initialization(0, vector, size, arg);

	args.inf = 0;
	args.sup = size - 1;
	args.vector = vector;
	args.size = size;

	dowork(&args);

	collect_results(0, &args);
}
