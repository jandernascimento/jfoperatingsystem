#include "reader_writer_3.h"
#include "reader_writer_tracing.h"

extern tracing_t t;

pthread_mutex_t mutex,condition_mutex;
pthread_cond_t condition;
int writing;
int reading;
//thread_info_t tinfo;
thread_stats_t stats;

thread_stats_t addThreadStats(int tid) {

    thread_stats_t result = malloc(sizeof(thread_stats_s));
    result->tid = tid;
    result->writes = 0;
    result->reads = 0;
    result->next = NULL;

    thread_stats_t ptr = stats;
    if (ptr != NULL) {
        while (ptr->next != NULL) {
            ptr = (thread_stats_t) ptr->next;
        }
        ptr->next = (thread_stats_t) result;
    } else {
        stats = result;
    }

    return result;
}

thread_stats_t findStats(int tid) {
    thread_stats_t ptr = stats;
    while (ptr != NULL) {
        if (ptr->tid == tid) {
            printf("findStats(): FOUND STATS FOR Thread %x\n", (unsigned int) tid);
            return ptr;
        }
        ptr = ptr->next;
    }
    printf("findStats(): FOUND NO STATS FOR Thread %x\n", (unsigned int) tid);
    return NULL;
}

int writeIsMoreFair(int tid) {
    printf("Thread %x BEGIN WRITE IS MORE FAIR\n", (unsigned int) tid);
    thread_stats_t tid_stats = findStats(tid);
    if (tid_stats == NULL) {
        addThreadStats(tid);
    }
    printf("STEP: 1\n");
    int counter = 0;
    int min = -1, val = -1;

    thread_stats_t ptr = stats;
    while (ptr != NULL) {
        counter++;
        if (ptr->tid == tid) {
            val = ptr->writes;
        }
        if (min == -1) {
            min = ptr->writes;
        } else if (min > ptr->writes) {
            min = ptr->writes;
        }
        ptr = ptr->next;
    }
    printf("MIN: %d, val: %d\n", min, val);
    if (val == -1 || min == -1) {
        return 1;
    } else {
        printf("Thread %x END WRITE IS MORE FAIR RESULT=%d\n", (unsigned int) tid, (val==min));
        return (val == min);
    }


}



//thread_info_t addThreadInfo(int tid) {
//
//    thread_info_t result = malloc(sizeof(thread_info_s));
//    result->tid = tid;
//    result->priority = 0;
//
//    thread_info_t ptr = tinfo;
//    if (ptr != NULL) {
//        while (ptr->next != NULL) {
//            ptr = (thread_info_t) ptr->next;
//        }
//        ptr->next = (thread_info_t) result;
//    } else {
//        tinfo = result;
//    }
//
//    return result;
//}
//
//}
//void removeThreadInfo(int tid) {
//
//    thread_info_t ptr = tinfo, prev_ptr = NULL;
//    if (ptr != NULL) {
//        while (ptr->next != NULL) {
//            prev_ptr = ptr;
//            ptr = ptr->next;
//            if (ptr->tid == tid) {
//                if (prev_ptr == NULL) {
//                    tinfo = ptr->next;
//                    return;
//                }
//                prev_ptr->next = ptr->next;
//                free(ptr);
//                return;
//            }
//        }
//    }
//}
//
//int hasHighestPriority(int tid) {
//
//    int priority = -1;
//    int maxPriority = -1;
//    thread_info_t ptr = tinfo;
//    printf("TEST PRIORITY\n");
//    if (ptr != NULL) {
//        while (ptr->next != NULL) {
//            if (ptr->priority > maxPriority) {
//                maxPriority = ptr->priority;
//            }
//            if (ptr->tid == tid) {
//                priority = ptr->priority;
//            }
//            ptr = ptr->next;
//        }
//    }
//
//    return (priority >= maxPriority);
//}

void init(reader_writer_t rw){
    printf("TESTINIT\n");
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&condition, NULL);
    writing = 0;
    reading = 0;
}

void begin_read(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();

//  thread_info_t tinfo = addThreadInfo(tid);
//  printf("BEGIN---READ\n");
  while (writing == 1) {
//    printf("Thread %x is waiting, priority : %d\n", (unsigned int) tid, tinfo->priority);
    pthread_cond_wait(&condition, &condition_mutex);
//    tinfo->priority++;

  }
  pthread_mutex_lock(&mutex);
  ++reading;
  tracing_record_event(t, BR_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void end_read(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, ER_EVENT_ID);
  --reading;
//  removeThreadInfo(tid);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void begin_write(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
//  thread_info_t tinfo = addThreadInfo(tid);
//  while (writing == 1 && reading==1 && !writeIsMoreFair(tid)) {
  while (!writeIsMoreFair(tid)) {
    pthread_cond_wait(&condition, &condition_mutex);
//    tinfo->priority++;
  }
  pthread_mutex_lock(&mutex);
  writing = 1;
  thread_stats_t tid_stats = findStats(tid);
  tid_stats->writes++;
  tracing_record_event(t, BW_EVENT_ID);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void end_write(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, EW_EVENT_ID);
  writing = 0;
//  removeThreadInfo(tid);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

