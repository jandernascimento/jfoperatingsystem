#include "reader_writer_3.h"
#include "reader_writer_tracing.h"

extern tracing_t t;

pthread_mutex_t mutex,condition_mutex;
pthread_cond_t condition;
int writing;
int reading;
thread_info_t threadinfos;
//thread_stats_t stats;

//thread_stats_t addThreadStats() {
//    printf("BEGIN addThreadStats()\n");
//    thread_stats_t result = malloc(sizeof(thread_stats_s));
//    result->tid=-1;
//    result->writes = 0;
//    result->reads = 0;
//    result->next = NULL;
//
//    thread_stats_t ptr = stats;
//    if (ptr != NULL) {
//        while (ptr->next != NULL) {
//            ptr = (thread_stats_t) ptr->next;
//        }
//        ptr->next = (thread_stats_t) result;
//    } else {
//        stats = result;
//    }
//    printf("END addThreadStats()\n");
//    return result;
//}
//
//thread_stats_t findThreadStats(int tid) {
//    thread_stats_t ptr = stats;
//    while (ptr != NULL) {
//        if (ptr->tid == tid) {
//            printf("findStats(): FOUND STATS FOR Thread %x\n", (unsigned int) tid);
//            return ptr;
//        }
//        ptr = ptr->next;
//    }
//    printf("findStats(): FOUND NO STATS FOR Thread %x\n", (unsigned int) tid);
//    return NULL;
//}
//
//thread_stats_t findUnusedThreadStats() {
//    thread_stats_t ptr = stats;
//    while (ptr != NULL) {
//        if (ptr->tid == -1) {
//            return ptr;
//        }
//        ptr = ptr->next;
//    }
////    printf("findStats(): FOUND NO STATS FOR Thread %x\n", (unsigned int) tid);
//    return NULL;
//}
//
//void checkThreadStats(int tid) {
//    printf("BEGIN CHECK_THREAD_STATS\n");
//    thread_stats_t threadstats;
//    if (findThreadStats(tid) == NULL) {
////        threadstats = findUnusedThreadStats();
//        threadstats = addThreadStats(tid);
//        threadstats->tid = tid;
//    }
//}
//
//int writeIsMoreFair(int tid) {
//    printf("Thread %x BEGIN WRITE IS MORE FAIR\n", (unsigned int) tid);
//    thread_stats_t tid_stats = findThreadStats(tid);
//    if (tid_stats == NULL) {
//        addThreadStats(tid);
//    }
//    printf("STEP: 1\n");
//    int counter = 0;
//    int min = -1, val = -1;
//
//    thread_stats_t ptr = stats;
//    while (ptr != NULL) {
//        counter++;
//        if (ptr->tid == tid) {
//            val = ptr->writes;
//        }
//        if (min == -1) {
//            min = ptr->writes;
//        } else if (min > ptr->writes) {
//            min = ptr->writes;
//        }
//        ptr = ptr->next;
//    }
//    printf("MIN: %d, val: %d\n", min, val);
//    if (val == -1 || min == -1) {
//        return 1;
//    } else {
//        printf("Thread %x END WRITE IS MORE FAIR RESULT=%d\n", (unsigned int) tid, (val==min));
//        return (val == min);
//    }
//
//
//}

void resetPriorities(int tid) {
    thread_info_t ptr = threadinfos;
    while (ptr != NULL) {
        if (ptr->tid == tid) {
            ptr->priority = 0;
            printf("resetPriorities: %x reset to 0\n", (unsigned int) tid);
        } else {
            ptr->priority++;
            printf("resetPriorities: %x increased to %d\n", (unsigned int) ptr->tid, ptr->priority);

        }

        ptr = ptr->next;
    }
    printf("resetPriorities(): Priorities reset\n");
}

thread_info_t addThreadInfo(int tid) {

    thread_info_t result = malloc(sizeof(thread_info_s));
    result->tid = tid;
    result->priority = 0;

    thread_info_t ptr = threadinfos;
    if (ptr != NULL) {
        while (ptr->next != NULL) {
            ptr = (thread_info_t) ptr->next;
        }
        ptr->next = (thread_info_t) result;
    } else {
        threadinfos = result;
    }
    sleep(0.5);
    return result;
}

thread_info_t findThreadInfo(int tid) {
    printf("BEGIN findThreadInfo()\n");
    thread_info_t ptr = threadinfos;
    while (ptr != NULL) {
        if (ptr->tid == tid) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void checkThreadInfo(int tid) {
    printf("BEGIN checkThreadInfo() Thread: %x\n", (unsigned int) tid);
    if (findThreadInfo(tid) == NULL) {
        addThreadInfo((tid));
        printf("checkThreadInfo(): ThreadInfo added\n");
    } else {
        printf("checkThreadInfo(): No ThreadInfo needs to be added\n");
    }
    printf("END checkThreadInfo()\n");
}

void removeThreadInfo(int tid) {
    printf("BEGIN removeThreadInfo()\n");
    thread_info_t ptr = threadinfos, prev_ptr = NULL;
    if (ptr != NULL) {
        while (ptr->next != NULL) {
            prev_ptr = ptr;
            ptr = ptr->next;
            if (ptr->tid == tid) {
                printf("removeThreadInfo(): Will remove listitem of Thread %x\n", (unsigned int) tid);
                if (prev_ptr == NULL) {
                    threadinfos = ptr->next;
                    return;
                }
                prev_ptr->next = ptr->next;
                free(ptr);
                return;
            }
        }
        printf("removeThreadInfo(): Didn't find any listitem to be removed for Thread %x", (unsigned int) tid);
    }
    printf("END removeThreadInfo()\n");
}

int hasHighestPriority(int tid) {
    printf("BEGIN hasHighestPriority()\n");
    thread_info_t threadinfo = findThreadInfo(tid);
    int priority;
    if (threadinfo != NULL) {
        priority = threadinfo->priority;
    } else {
        printf("ERROR: hasHighestPriority(): No Thread with Signature %x found\n", (unsigned int) tid);
        return -1;
    }
    int maxPriority = -1;
    thread_info_t ptr = threadinfos;

    if (ptr != NULL) {
        while (ptr->next != NULL) {
            if (ptr->priority > maxPriority) {
                maxPriority = ptr->priority;
            }
            if (ptr->tid == tid) {
                priority = ptr->priority;
            }
            ptr = ptr->next;
        }
    }
    printf("END hasHighestPriority(), %d vs max=%d, RESULT=%d\n", priority, maxPriority, (priority >= maxPriority));

    return (priority >= maxPriority);
}

void init(reader_writer_t rw){
    printf("BEGIN INIT\n");
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&condition, NULL);
    writing = 0;
    reading = 0;
    printf("END INIT\n");
}

void begin_read(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
  checkThreadInfo(tid);
//  while (writing == 1 || !hasHighestPriority(tid)) {
  while (!hasHighestPriority(tid)) {
    pthread_cond_wait(&condition, &condition_mutex);
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
  pthread_mutex_lock(&mutex);
  tracing_record_event(t, ER_EVENT_ID);
  --reading;
  int tid = pthread_self();
  removeThreadInfo(tid);
  resetPriorities(tid);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}

void begin_write(reader_writer_t rw){
  /* ... */
  pthread_mutex_lock(&condition_mutex);
  int tid = pthread_self();
  checkThreadInfo(tid);
//  while (writing == 1 || reading>0 || !hasHighestPriority(tid)) {
  while (!hasHighestPriority(tid)) {
    pthread_cond_wait(&condition, &condition_mutex);
  }
  pthread_mutex_lock(&mutex);
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
  removeThreadInfo(tid);
  resetPriorities(tid);
  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&condition);
  pthread_mutex_unlock(&condition_mutex);
  /* ... */
}



