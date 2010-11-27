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
  sleep(1);
  for(i = 0; i < 10; i++){
    list_insert(&list, rand()%10); 
    if(list_exists(&list, id)){
      printf("Thread nmbr %d, won.\n", id); 
      return NULL; 
    }
  }
  return NULL;
}

//BR_EVENT_ID 0
//ER_EVENT_ID 1
//BW_EVENT_ID 2
//EW_EVENT_ID 3
void tst(sevent_t events, int size){

	int x;
	//counter for sequencial BR
	int seq_BR_counter=0;
	//counter for sequencial ER
	int seq_ER_counter=0;
	for(x=0;x<size;x++){

		int event=events[x].event;
		int thread=events[x].thread;

		if(event==ER_EVENT_ID||event==EW_EVENT_ID) continue;

		//Case an event begin read, the next must be end event end read for the same thread
		if(event==BR_EVENT_ID){
			int k=0;

			//There MUST NOT exist a write in reads
			for(k=x+1;k<size;k++){
				
				if(events[k].event==ER_EVENT_ID) break;	
				
				if(events[k].event==BW_EVENT_ID || events[k].event==EW_EVENT_ID ){
					printf("Test FAIL at line #%i\n",x);
					exit(EXIT_FAILURE);
				}
			}
			
			//Counts the sequencial readings
			if(events[x+1].event==event && event==BR_EVENT_ID){
				seq_BR_counter++;
			}
			if(events[x+1].event==event && event==ER_EVENT_ID){
				seq_ER_counter++;
			}


		}	
		//Case an event begin write, the next must be end event end write for the same thwrite
		if(event==BW_EVENT_ID){
			if(events[x+1].thread!=thread||events[x+1].event!=EW_EVENT_ID){
				printf("Test FAIL at line #%i!\n",x);
				exit(EXIT_FAILURE);
			}
		}
	
	}

	if(seq_BR_counter<2 && seq_ER_counter<2){
		printf("FAIL! No double readings, the output does not proof that multiples readings are allowed!\n");
		exit(EXIT_FAILURE);
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

/** CHECK: BEGIN **/

//check_error(&tracing,&tst);
simpler_log(&tracing,&tst);

/** CHECK: END **/
//print_all_events_per_date(t); 
//  exit(EXIT_FAILURE); 
}
