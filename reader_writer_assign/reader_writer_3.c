#include "reader_writer_3.h"
#include "reader_writer_tracing.h"

extern tracing_t t;

pthread_mutex_t mutex,condition_mutex;
pthread_cond_t condition;
pthread_cond_t condition_readwrite;
pthread_cond_t condition_queue;
int writing;
int reading;
thread_queue_t queue;


void init(reader_writer_t rw){
    printf("BEGIN INIT\n");
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&condition_readwrite, NULL);
    pthread_cond_init(&condition_queue, NULL);
    queue = malloc(sizeof(thread_queue_s));
    writing = 0;
    reading = 0;
    printf("END INIT\n");
}

int isFirstInQueue(int tid) {
    if (queue->first_thread != NULL) {
        printf("isFirstinQueue(): Current Thread: %x, First Thread %x, Result: %d\n", (unsigned int) tid, queue->first_thread->tid, queue->first_thread->tid == tid);
        return (queue->first_thread->tid == tid);
    }
    printf("isFirstinQueue(): No appropriate Thread with ID %x found\n", (unsigned int) tid);
    return 1;
}

void throwOutOfQueue() {
    printf("throwOutOfQueue() BEGIN\n");
    waiting_thread_t thread = queue->first_thread;
    queue->first_thread = thread->next;
    printf("throwOutOfQueue(): Thread with ID %x thrown out\n", (unsigned int) thread->tid);
    free(thread);
    printf("throwOutOfQueue() END\n");
}

waiting_thread_t addToQueue(int tid) {
    printf("addToQueue() BEGIN\n");
    waiting_thread_t thread = malloc(sizeof(waiting_thread_s));
    thread->tid = tid;
    waiting_thread_t ptr = queue->first_thread;
    printf("addToQueue() STEP 1\n");
    if (ptr != NULL) {
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        printf("addToQueue() STEP 2\n");
        ptr->next = thread;
    } else {
        queue->first_thread = thread;
    }
    printf("addToQueue() Thread with ID %x added to Queue\n", (unsigned int) tid);
    printf("addToQueue() END\n");
}

void begin_read(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
  addToQueue(tid);
  while (writing == 1 || !isFirstInQueue(tid)) {
//  while (!hasHighestPriority(tid)) {
    pthread_cond_wait(&condition, &condition_mutex);
  }
  while (pthread_mutex_trylock(&mutex)) {
    pthread_mutex_unlock(&condition_mutex);
    /* stall here */
    sleep(3);
    pthread_mutex_lock(&condition_mutex);
  }

//  pthread_mutex_lock(&mutex);
  ++reading;
  tracing_record_event(t, BR_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void end_read(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, ER_EVENT_ID);
  --reading;
  int tid = pthread_self();
  throwOutOfQueue();
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void begin_write(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
  addToQueue(tid);
  while (writing == 1 || reading>0 || !isFirstInQueue(tid)) {
    pthread_cond_wait(&condition, &condition_mutex);
  }
  while (pthread_mutex_trylock(&mutex)) {
    pthread_mutex_unlock(&condition_mutex);
    /* stall here */
    sleep(3);
    pthread_mutex_lock(&condition_mutex);
  }
//  pthread_mutex_lock(&mutex);
  writing = 1;
  tracing_record_event(t, BW_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void end_write(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, EW_EVENT_ID);
  writing = 0;
  int tid = pthread_self();
  throwOutOfQueue();
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}



