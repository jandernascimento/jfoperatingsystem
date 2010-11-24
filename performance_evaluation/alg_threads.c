#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "alg_principal.h"
#include "call_sequential.h"

void *thread_function(void *arg) {
	dowork((arguments_t *) arg);
	pthread_exit(NULL);
}

void call_sort(int parallelism, int *vector, int size, char *arg) {
	arguments_t *arguments;
	pthread_t *tids;
	int inf, sup;
	int err, i;
	void *status, *args_algo;

	args_algo = initialization(parallelism, vector, size, arg);

	tids = malloc(parallelism * sizeof(pthread_t));
	arguments = malloc(parallelism * sizeof(arguments_t));
	if ((tids == NULL) || (arguments == NULL)) {
		fprintf(stderr, "malloc failed for thread data structures\n");
		exit(1);
	}

	inf = 0;
	sup = (size - 1) / parallelism;

	for (i = 0; i < parallelism; i++) {

		arguments[i].inf = inf;
		arguments[i].sup = sup;
		arguments[i].vector = vector;
		arguments[i].size = size;

		err
				= pthread_create(&tids[i], NULL, thread_function,
						&arguments[i]);
		if (err != 0) {
			fprintf(stderr, "error while creating thread id %d\n", i);
			exit(1);
		}
		inf = sup + 1;
		sup = ((i + 2) * (size - 1)) / parallelism;
	}

	for (i = 0; i < parallelism; i++) {
		pthread_join(tids[i], &status);
	}
	collect_results(parallelism, arguments);

	free(arguments);
	free(tids);
}



