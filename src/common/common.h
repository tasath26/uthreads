#ifndef _RV_COMMON_H_
#define _RV_COMMON_H_

#define FAILURE -1

typedef struct uthread uthread_t;
typedef struct ut_queue ut_queue_t;

extern uthread_t *current_thread;

extern ut_queue_t thread_queue;

void uthread_switch(void);

#endif 
