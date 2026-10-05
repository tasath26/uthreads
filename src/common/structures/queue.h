#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include "../common.h"

typedef struct ut_queue{
    uthread_t   **thread;
    size_t        capacity;
    size_t        head;
    size_t        tail;
    size_t        count;
} ut_queue_t;

int ut_queue_init(ut_queue_t *q);

int ut_queue_enq(ut_queue_t *q, uthread_t *thread);

uthread_t *ut_queue_deq(ut_queue_t *q);

void ut_queue_destroy(ut_queue_t *q);

int ut_queue_empty(const ut_queue_t *q);
  
#endif 
