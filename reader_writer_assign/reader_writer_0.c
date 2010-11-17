#include "reader_writer.h"
#include "reader_writer_tracing.h"
extern tracing_t t; 

void init(reader_writer_t rw){

}

void begin_read(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, BR_EVENT_ID);

  /* ... */

		       
}

void end_read(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, ER_EVENT_ID);  

  /* ... */


}

void begin_write(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, BW_EVENT_ID);  

  /* ... */


  
}

void end_write(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, EW_EVENT_ID);  

  /* ... */


  
}

