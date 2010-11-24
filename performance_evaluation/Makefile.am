AM_CFLAGS=-W -Wall -g

COMMONS=main.c measure.c alg_principal.h call_sequential.h measure.h

SORT=sort.c $(COMMONS)

bin_PROGRAMS= sort_seq sort_par

sort_par_SOURCES=alg_threads.c $(SORT)
sort_par_LDADD=-lpthread

sort_seq_SOURCES=alg_sequential.c $(SORT)

test: $(bin_PROGRAMS)
	./sort_seq --time --rusage --nsize 1000
	./sort_par --time --rusage --parallelism 4 --nsize 1000

EXTRA_DIST=run_exp1.sh
