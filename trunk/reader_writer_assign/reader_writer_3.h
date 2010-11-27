#ifndef   	_READER_WRITER_H_
#define   	_READER_WRITER_H_

#define BR_EVENT_ID 0
#define ER_EVENT_ID 1
#define BW_EVENT_ID 2
#define EW_EVENT_ID 3

typedef struct{
  /* ... */

}reader_writer_s, *reader_writer_t;

//typedef struct{
//    int tid;
//    int priority;
//    struct thread_info_s *next;
//}thread_info_s, *thread_info_t;

typedef struct{
    int tid;
    int reads;
    int writes;
    struct thread_stats_s *next;
}thread_stats_s, *thread_stats_t;

void init(reader_writer_t rw);

void begin_read(reader_writer_t rw);

void end_read(reader_writer_t rw);

void begin_write(reader_writer_t rw);

void end_write(reader_writer_t rw);


#endif 	    /* !_READER_WRITER_H_ */
