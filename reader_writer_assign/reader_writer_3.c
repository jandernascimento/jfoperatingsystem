#include "reader_writer_2.h"
#include "reader_writer_tracing.h"

extern tracing_t t;

pthread_mutex_t mutex,condition_mutex;
pthread_cond_t condition;
int writing;
int reading;
int deny_new_readers;

void init(reader_writer_t rw){
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&condition, NULL);
    writing = 0;
    reading = 0;
    deny_new_readers = 0;
}

void begin_read(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);

  while (writing == 1 || deny_new_readers == 1) {

    pthread_cond_wait(&condition, &condition_mutex);

  }
  ++reading;
  tracing_record_event(t, BR_EVENT_ID);

}

void end_read(reader_writer_t rw){

  tracing_record_event(t, ER_EVENT_ID);
  --reading;
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);


}

void begin_write(reader_writer_t rw){
  pthread_mutex_lock(&condition_mutex);
  pthread_mutex_lock(&mutex);
  while (writing == 1 || reading > 0) {
    if (reading>0) {
        deny_new_readers = 1;
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_wait(&condition, &condition_mutex);
    pthread_mutex_lock(&mutex);
  }
  writing = 1;
  tracing_record_event(t, BW_EVENT_ID);
}

void end_write(reader_writer_t rw){
  tracing_record_event(t, EW_EVENT_ID);
  writing = 0;
  deny_new_readers = 0;
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);

}

