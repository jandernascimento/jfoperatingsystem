#include "reader_writer.h"
#include "reader_writer_tracing.h"

extern tracing_t t;

pthread_mutex_t mutex,condition_mutex;
pthread_cond_t condition;
int writing;
int reading;

void init(reader_writer_t rw){
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&condition, NULL);
    writing = 0;
    reading = 0;
}

void begin_read(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  while (writing == 1) {
    pthread_cond_wait(&condition, &condition_mutex);
  }
  pthread_mutex_lock(&mutex);
  ++reading;
  tracing_record_event(t, BR_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
}

void end_read(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, ER_EVENT_ID);
  --reading;
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
}

void begin_write(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  while (writing == 1 || reading>0) {
    pthread_cond_wait(&condition, &condition_mutex);
  }
  pthread_mutex_lock(&mutex);
  writing = 1;
  tracing_record_event(t, BW_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
}

void end_write(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, EW_EVENT_ID);
  writing = 0;
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
}

