#include "../common/common.h"
#include "../common/structures/queue.h"
#include "../common/structures/table.h"
#include "../arch/arch.h"

#include "uthreads.h"

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

typedef struct uthread {
  unsigned int tid;
  uctx_t context;     // Struct content depends on architecture
  ustack_t stack;
  ufunc_t func;
} uthread_t;

uthread_t *current_thread = NULL;

ut_queue_t thread_queue = {0};

void uthread_init(void) {

  uthread_t *main_thread = calloc(1,sizeof(*main_thread));
  if(!main_thread){
    abort();
  }
  
  if(ut_table_init() == -1){
    abort();
  }

  if(ut_queue_init(&thread_queue) == -1){
    abort();
  }

  int tid = ut_table_alloc(main_thread);
  if (tid != 0) {
    abort();
  }

  main_thread->tid = 0;
  
  main_thread->stack.size = 0;
  main_thread->stack.base = NULL;

  current_thread = main_thread;

  return;
}

int uthread_new(int func(void *), void *args, size_t nbytes) {

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
  if (!func || (nbytes > 0 && !args)) {
      return FAILURE;
  }

  new_thread->func.start = func;
  new_thread->func.arg = NULL;

  if(nbytes > 0){
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

  // Context Initialization
  arch_context_init(&new_thread->context, new_thread->stack.base, new_thread->stack.size, new_thread->func.start, new_thread->func.arg);

  // Thread is in queue - READY STATE
  if (ut_queue_enq(thread_queue, new_thread) != 0) {
    free(new_thread->stack.base);
    free(new_thread->func.arg);
    ut_table_release(tid);
    free(new_thread);
    return FAILURE;
  }

  return tid;
}

int uthread_join(int tid) { return -1; }

int uthread_self(void) { return current_thread->tid; }

void uthread_pause(void) { return; }
