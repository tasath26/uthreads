#include "../arch/arch.h"
#include "../common/common.h"
#include "../common/structures/queue.h"
#include "../common/structures/table.h"

#include "uthreads.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define UTHREAD_STACK_SIZE (64 * 1024)

typedef struct ustack {
  void *base;
  size_t size;
} ustack_t;

typedef struct ufunc {
  int (*start)(void *);
  void *arg;
} ufunc_t;

typedef struct ujoin {
  int wait_tid;
  int res;
} ujoin_t;

typedef struct uthread {
  unsigned int tid;
  uctx_t context; // Struct content depends on architecture
  ustack_t stack;
  ufunc_t func;
  ujoin_t join;
} uthread_t;

uthread_t *current_thread = NULL;

ut_queue_t thread_queue = {0};
ut_queue_t blocked_queue = {0};
ut_queue_t exited_queue = {0};

#define UTHREADS_JOIN_WAIT_ALL -10
#define UTHREADS_NUM_LIVE (thread_queue.count + blocked_queue.count)

static inline void uthread_cleanup(void) {
  while (!ut_queue_empty(&exited_queue)) {
    uthread_t *thread = ut_queue_deq(&exited_queue);

    free(thread->func.arg);
    free(thread->stack.base);
    free(thread);
  }
}

void uthread_init(void) {

  uthread_t *main_thread = calloc(1, sizeof(*main_thread));
  if (!main_thread)
    abort();

  if (ut_table_init() == -1) {
    abort();
  }

  // Initialize the thread run queue
  if (ut_queue_init(&thread_queue) == -1) {
    abort();
  }

  // Initialize the blocked/waiting thread queue
  if (ut_queue_init(&blocked_queue) == -1) {
    abort();
  }

  // Initialize exited thread queue
  if (ut_queue_init(&exited_queue) == -1) {
    abort();
  }

  // Add thread to our global threads table
  int tid = ut_table_alloc(main_thread);
  if (tid != 0) {
    abort();
  }

  // Add thread to run queue
  if (ut_queue_enq(&thread_queue, main_thread) != 0) {
    abort();
  }

  // Initialize main thread
  main_thread->tid = 0;

  main_thread->join.wait_tid = -1;
  main_thread->join.res = 0;

  main_thread->stack.size = 0;
  main_thread->stack.base = NULL;

  current_thread = main_thread;

  return;
}

int uthread_new(int func(void *), void *args, size_t nbytes) {

  // Validate arguments
  if (!func || (nbytes > 0 && !args)) {
    return FAILURE;
  }

  uthread_t *new_thread = malloc(sizeof(*new_thread));
  if (!new_thread) {
    return FAILURE;
  }

  int tid = ut_table_alloc(new_thread);
  if (tid == -1) {
    free(new_thread);
    return FAILURE;
  }

  new_thread->tid = tid;

  // Function Initializations
  new_thread->func.start = func;
  new_thread->func.arg = NULL;

  if (nbytes > 0) {
    new_thread->func.arg = malloc(nbytes);
    if (!new_thread->func.arg) {
      ut_table_release(tid);
      free(new_thread);
      return FAILURE;
    }

    memcpy(new_thread->func.arg, args, nbytes);
  }

  // Stack Initialization
  new_thread->stack.size = UTHREAD_STACK_SIZE;
  new_thread->stack.base = malloc(new_thread->stack.size);
  if (!new_thread->stack.base) {
    free(new_thread->func.arg);
    ut_table_release(tid);
    free(new_thread);
    return FAILURE;
  }

  // Join info Initialization
  new_thread->join.wait_tid = -1;
  new_thread->join.res = 0;

  // Context Initialization
  arch_context_init(&new_thread->context, new_thread->stack.base,
                    new_thread->stack.size, new_thread->func.start,
                    new_thread->func.arg);

  // Thread is in queue - READY STATE
  if (ut_queue_enq(&thread_queue, new_thread) != 0) {
    free(new_thread->stack.base);
    free(new_thread->func.arg);
    ut_table_release(tid);
    free(new_thread);
    return FAILURE;
  }

  return tid;
}

static int uthread_check_active_joinall(void){
  for(size_t i = 0; i < blocked_queue.count; ++i){
       uthread_t *thread = blocked_queue.thread[i]; 
      
      if (thread->join.wait_tid == UTHREADS_JOIN_WAIT_ALL) {
        return 1;
      } 
  }

  return 0;
}

int uthread_join(int tid) {

  if (tid == 0) {

    if (UTHREADS_NUM_LIVE == 1) {
      return 0;
    }

    
    if (thread_queue.count <= 1){
      abort();
    }

    uthread_t * from = current_thread;
  
    // Check if we have another join all active
    if(uthread_check_active_joinall() == 1){
      abort();
    }

    from->join.wait_tid = UTHREADS_JOIN_WAIT_ALL;

    ut_queue_deq(&thread_queue);
    ut_queue_enq(&blocked_queue, from);
    
    uthread_t *to = thread_queue.thread[thread_queue.head];

    current_thread = to;
    _uthread_switch(&from->context, &to->context);

    uthread_cleanup();

    return 0;
  }

  uthread_t *target = ut_table_lookup(tid);
  if (target == NULL) {
    return -1;
  }
 
  if (thread_queue.count <= 1){
    abort();
  }

  uthread_t *from = current_thread;

  if (from == target) {
    abort();
  }

  if (ut_queue_enq(&blocked_queue, from) == -1) {
    abort();
  }

  from->join.wait_tid = tid;
  ut_queue_deq(&thread_queue);

  uthread_t *to = thread_queue.thread[thread_queue.head];

  current_thread = to;
  _uthread_switch(&from->context, &to->context);

  uthread_cleanup();
  return from->join.res;
}

int uthread_self(void) { return current_thread->tid; }

void uthread_pause(void) {

  if (thread_queue.count <= 1) {
    return;
  }

  uthread_t *from = ut_queue_deq(&thread_queue);
  uthread_t *to = thread_queue.thread[thread_queue.head];

  if (ut_queue_enq(&thread_queue, from) == -1) {
    abort();
  }

  current_thread = to;
  _uthread_switch(&from->context, &to->context);

  uthread_cleanup();
}

void uthread_exit(int code) {

  uthread_cleanup();

  uthread_t *from = current_thread;

  // Add waiting threads back to run queue
  for (size_t i = 0; i < blocked_queue.count; ++i) {
    uthread_t *waiting = ut_queue_deq(&blocked_queue);

    if (waiting->join.wait_tid == current_thread->tid) {
      waiting->join.res = code;
      waiting->join.wait_tid = -1;

      ut_queue_enq(&thread_queue, waiting);

    } else if (waiting->join.wait_tid == UTHREADS_JOIN_WAIT_ALL){
      waiting->join.res = code; 
      waiting->join.wait_tid = -1;
      
      if(UTHREADS_NUM_LIVE == 1){
         ut_queue_enq(&thread_queue, waiting);
      }

    } else {
      ut_queue_enq(&blocked_queue, waiting);
    }
  }

  ut_queue_deq(&thread_queue);
  ut_table_release(current_thread->tid);

  if (ut_queue_empty(&thread_queue)) {
    // Threads left in the blocked queue
    if (!ut_queue_empty(&blocked_queue)) {
      abort();
    }

    // Exiting if all threads are finished
    exit(0);
  }

  ut_queue_enq(&exited_queue, from);

  uthread_t *to = thread_queue.thread[thread_queue.head];

  current_thread = to;
  _uthread_switch(&from->context, &to->context);

  // Exiting thread shouldn't resume executing
  abort();
}
