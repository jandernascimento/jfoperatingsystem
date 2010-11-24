#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "call_sequential.h"

int compar(const void *a, const void *b) {
	return *(int *) a - *(int *) b;
}

void *initialization(int parallelism, int *vector, int size, char *arg) {
	// just for reducing compiler warnings
	(void) parallelism;
	(void) vector;
	(void) size;
	(void) arg;
	return NULL;
}

void dowork(arguments_t *args) {
	qsort(args->vector + args->inf, args->sup - args->inf + 1, sizeof(int),
			compar);
}

void collect_results(int parallelism, arguments_t *args) {
	int i, j, min;

	/* Ici on fusionne les sous tableaux tries */
	if (parallelism > 1) {
		int *resultat;
		resultat = malloc(sizeof(int) * args->size);
		if (resultat == NULL) {
			fprintf(stderr, "Impossible d'allouer le tableau resultat\n");
			exit(1);
		}

		/* Remarquons que le coût de cette fusion est en parallelisme*taille */
		for (i = 0; i < args->size; i++) {
			min = -1;
			for (j = 0; j < parallelism; j++)
				if ((args[j].inf <= args[j].sup) && ((min == -1)
						|| (args->vector[args[j].inf]
								< args->vector[args[min].inf])))
					min = j;
			resultat[i] = args->vector[args[min].inf++];
		}
		memcpy(args->vector, resultat, sizeof(int) * args->size);
	}
}
