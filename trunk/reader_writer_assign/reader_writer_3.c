#include "reader_writer.h"
#include "reader_writer_tracing.h"

extern tracing_t t;

pthread_mutex_t mutex,condition_mutex;
pthread_cond_t condition;
int *writing=NULL;
int *reading=NULL;

void init(reader_writer_t rw){
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&condition, NULL);
}

void init_local(){
  int total=t->nb_threads;
  int x=0;
  if(writing==NULL){
	writing=malloc(sizeof(int)*total);
        for(x=0;x<total;x++) writing[x]=0;
  }
  if(reading==NULL){
	reading=malloc(sizeof(int)*total);
        for(x=0;x<total;x++) reading[x]=0;
  }
}


int isSmallerReader(int tid){
    int x;
    int currentValue=reading[tid];
    int isSmaller=1;
    for(x=0;x<t->nb_threads;x++){
       if(x==tid) continue;
       
       if(reading[x]<currentValue) {
		isSmaller=0;
		break;  
       }
    }
    return isSmaller;
}


int isSmallerWriter(int tid){
    int x;
    int currentValue=writing[tid];
    int isSmaller=1;
    for(x=0;x<t->nb_threads;x++){
       if(x==tid) continue;
       
       if(writing[x]<currentValue) {
		isSmaller=0;
		break;  
       }
    }
    return isSmaller;
}

void begin_read(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  init_local();
  int tid=rw->nb;
  while (!isSmallerReader(tid)) {
    //printf("READ: esperando... %i .. %i\n",tid,reading[tid]);
    pthread_cond_wait(&condition, &condition_mutex);
    //printf("READ: acordando... %i \n",tid);
  }
  pthread_mutex_lock(&mutex);
  reading[tid]=reading[tid]+1;
  //printf("READ: entrou... %i .. quantidade: %i\n",tid,reading[tid]);
  tracing_record_event(t, BR_EVENT_ID);
}

void end_read(reader_writer_t rw){
  //printf("READ: saiu... %i\n",rw->nb);
  tracing_record_event(t, ER_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
  pthread_cond_signal(&condition);

}

void begin_write(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  init_local();
  int tid = rw->nb;
  while (!isSmallerWriter(tid) ) {
    //printf("WRITE: esperando... %i ... %i \n",tid,writing[tid]);
    pthread_cond_wait(&condition, &condition_mutex);
    //printf("WRITE: acordando... %i \n",tid);
  }
  writing[tid] = writing[tid]+1;
  //printf("WRITE: entrou... %i .. quantidade: %i\n",tid,writing[tid]);
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, BW_EVENT_ID);
}

void end_write(reader_writer_t rw){
  //printf("WRITE: saiu... %i\n",rw->nb);
  tracing_record_event(t, EW_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
  pthread_cond_signal(&condition);
}

