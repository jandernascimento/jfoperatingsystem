#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <getopt.h>
#include <sys/resource.h>

#include "alg_principal.h"
#include "measure.h"

void usage(char *commande) {
    fprintf(stderr, "Usage :\n");
    fprintf(stderr, "%s --nsize number [ --parallelism number ] [ --verbose] "
                    "[ --rusage ] "
    				"[ --seed number ]"
    		        "[ --help ]\n\n",
                    commande);
    exit(1);
}

int quiet=0;

int main(int argc, char *argv[]) {
    int opt, parallelism = 1;
    int size = 0, i, ressources = 0;
    int *vector;
    int verbose = 0;
    struct timeval s_time, e_time;
    char *arg=NULL;
    struct rusage rusage;
    time_t system, user;
    int seed = 0, seed_set = 0;

    struct option longopts[] = {
        { "help", required_argument, NULL, 'h' },
        { "parallelism", required_argument, NULL, 'p' },
        { "nsize", required_argument, NULL, 'n' },
        { "verbose", no_argument, NULL, 'v' },
        { "rusage", no_argument, NULL, 'r' },
        { "seed", required_argument, NULL, 's' },
        { "arg", required_argument, NULL, 'a' },
        { NULL, 0, NULL, 0 }
    };

    while ((opt = getopt_long(argc, argv, "hp:s:n:vra:", longopts, NULL)) != -1) {
        switch (opt) {
          case 'p':
            parallelism = atoi(optarg);
            break;
          case 'v':
            verbose = 1;
            break;
          case 'a':
            arg = optarg;
            break;
          case 'r':
             ressources = 1;
             break;
          case 'n':
            size = atoi(optarg);
            break;
          case 's':
        	seed = atoi(optarg);
        	seed_set = 1;
        	break;
          case 'h':
          default:
            usage(argv[0]);
        }
    }


    if( size <= 0 ) {
    	fprintf(stderr, "invalid size of vector %d\n", size);
    	usage(argv[0]);
    	exit(1);
    }

    vector = (int *) malloc(size*sizeof(int));
    if (vector == NULL) {
        fprintf(stderr,"malloc error\n");
        exit(3);
    }

    if( seed_set != 1  ) {
    	seed = time(NULL);
    }
    srandom(seed);
    for (i=0; i<size; i++) {
    	vector[i] = random() % size;
    }

    getrusage(RUSAGE_SELF, &rusage);
    user   = to_usec(rusage.ru_utime);
    system = to_usec(rusage.ru_stime);

    gettimeofday(&s_time, NULL);

    call_sort(parallelism, vector, size, arg);

    gettimeofday(&e_time, NULL);

    getrusage(RUSAGE_SELF, &rusage);
    user   = to_usec(rusage.ru_utime) - user;
    system = to_usec(rusage.ru_stime) - system;

    printf("p: %d\n", parallelism);
    printf("n: %d\n", size);
    printf("total time: %lld\n", to_usec(e_time) - to_usec(s_time));

    if( 1 == verbose ) {
		printf("Sorted vector :\n");
		for (i=0; i<size; i++)
			printf("%d ", vector[i]);
		printf("\n");
    }


    if (ressources) {
        printf("user       : %ld\n", user);
        printf("system     : %ld\n", system);
        printf("tot (u+s)  : %ld\n", user+system);
    }

    free(vector);

    return 0;
}
