#include "reader_writer_1.h"
#include "reader_writer_tracing.h"
#include "stdio.h"
extern tracing_t t;

pthread_mutex_t mutex;

void init(reader_writer_t rw){
  pthread_mutex_init(&mutex,NULL);
}

void begin_read(reader_writer_t rw){
  pthread_mutex_lock( &mutex );
  tracing_record_event(t, BR_EVENT_ID);
}

void end_read(reader_writer_t rw){
  pthread_mutex_unlock( &mutex );
  tracing_record_event(t, ER_EVENT_ID);
}

void begin_write(reader_writer_t rw){
  pthread_mutex_lock( &mutex );
  tracing_record_event(t, BW_EVENT_ID);
}

void end_write(reader_writer_t rw){
  pthread_mutex_unlock( &mutex );
  tracing_record_event(t, EW_EVENT_ID);
}

