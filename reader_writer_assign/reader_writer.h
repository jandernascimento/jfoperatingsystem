#ifndef   	_READER_WRITER_H_
#define   	_READER_WRITER_H_

#define BR_EVENT_ID 0
#define ER_EVENT_ID 1
#define BW_EVENT_ID 2
#define EW_EVENT_ID 3

typedef struct{
  /* ... */
  int nb;

}reader_writer_s, *reader_writer_t;

void init(reader_writer_t rw); 

void begin_read(reader_writer_t rw);

void end_read(reader_writer_t rw); 

void begin_write(reader_writer_t rw); 

void end_write(reader_writer_t rw); 


#endif 	    /* !_READER_WRITER_H_ */
