/* 
Create n thread inserting random values in a global list 
Whenever a thread found its id in the list, it returns. 
If it doesn't find its id after 10 insertions
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "linked_list.h"
#include "reader_writer.h"
#include "reader_writer_tracing.h"

#include <limits.h>
#include <string.h>

struct linked_list_head list; 
tracing_t t; 

void *thread_func(void *a){
  int id = (long int)a; 
  /* mandatory ! */
  tracing_register_thread(t, id); 
  int i ;
  for(i = 0; i < 10; i++){
    list_insert(&list, id);//rand()%10); 
    if(list_exists(&list, id)){
      //printf("Thread nmbr %d, won.\n", id); 
      //return NULL; 
    }
  }
  return NULL;
}

//BR_EVENT_ID 0
//ER_EVENT_ID 1
//BW_EVENT_ID 2
//EW_EVENT_ID 3
void fairness_tst(sevent_t events, int size){
	int reading[t->nb_threads];
	int writing[t->nb_threads];
   	int count=0;
	for(count=0;count<t->nb_threads;count++){
		reading[count]=0;
		writing[count]=0;
	}
	//This loop, counts number of the same operation performed by a same given thread
	int x;
	if(t->nb_threads>1)
	for(x=0;x<size;x++){
		int event=events[x].event;
		int thread=events[x].thread;
		if(thread==events[x+1].thread && event==BR_EVENT_ID){
			int total=1;
			int c=0;
			for(c=x+1;events[c].thread==thread;c++){
				if(events[c].event==BR_EVENT_ID)
					total=total+1;
			}
			if(total>reading[thread]) 			
				reading[thread]=total;
		}

		if(thread==events[x+1].thread && event==BW_EVENT_ID){
			int total=1;
			int c=0;
			for(c=x+1;events[c].thread==thread;c++){
				if(events[c].event==BW_EVENT_ID)
					total=total+1;
			}
			if(total>writing[thread]) 			
				writing[thread]=total;
		}
	}
	printf("Max amount of sequencial operation by thread:\n");
	int total_reading=0;
	int total_writing=0;
        for(count=0;count<t->nb_threads;count++){
		total_reading+=reading[count];
		total_writing+=writing[count];
		printf("THREAD %i Reading:%i Writing:%i\n",count,reading[count],writing[count]);
	}
	//mean of sequencial reading
	double mean_read=(double)(total_reading/t->nb_threads);
	double mean_write=(double)(total_writing/t->nb_threads);

	if(mean_read>3 || mean_write>3){
		printf("Test FAIL. The mean of sequencial read/write operation is superior than 3[mean read=%f,mean write=%f]!\n",mean_read,mean_write);
		exit(EXIT_FAILURE);
	}else{
		printf("Mean read=%f Mean write=%f\n",mean_read,mean_write);
	}
	
}

int main(int argc, char **argv){
  
  if(argc != 2){
    printf("usage : %s nb_threads\n", argv[0]); 
    exit(1);
  }

  int nb_threads = atoi(argv[1]); 
  
  srand(time(NULL)*getpid()); 

  /*** Linked list ***/
  list_init(&list); 

  /*** Tracing ... ***/
  tracing_s tracing; 
  t = &tracing; 

  /* set a timeout to kill the process in case of deadlock */
  alarm(5); 
  tracing_init(t, nb_threads); 

  /* Give a human readable name to events */
  tracing_register_event(t, BR_EVENT_ID, "BEGIN READ");
  tracing_register_event(t, ER_EVENT_ID, "END READ");
  tracing_register_event(t, BW_EVENT_ID, "BEGIN WRITE");
  tracing_register_event(t, EW_EVENT_ID, "END WRITE");

  /*** Thread creation ***/
  pthread_t *tids = malloc(sizeof(pthread_t) * nb_threads -1); 
  long int i; 

  for(i = 0; i < nb_threads-1; i++){
    pthread_create(tids+i, NULL, thread_func, (void*)(i+1)); 
  }

  thread_func((void*)0); 

  for(i = 0; i < nb_threads-1; i++){
    pthread_join(tids[i], NULL); 
  }

  //check_error(&tracing,&tst);
  print_all_events_per_date(t); 
  simpler_log(&tracing,&fairness_tst);
  exit(EXIT_SUCCESS); 
}
