#include "../common/common.h"
#include "../common/structures/queue.h"
#include "../common/structures/table.h"
#include "uthreads.h"

#include <stdint.h>
#include <stdlib.h>

#define UTHREAD_STACK_SIZE (64 * 1024);

typedef struct uctx {
  uint64_t rsp;
  uint64_t rbx;
  uint64_t rbp;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t rdi;
} uctx_t;

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
  uctx_t context;
  ustack_t stack;
  ufunc_t func;
} uthread_t;

uthread_t *current_thread = NULL;

ut_queue_t *thread_queue = NULL;

void uthread_init(void) {

  uthread_t *main_thread = malloc(sizeof(*main_thread));

  ut_table_init();
  ut_queue_init(thread_queue);

  int tid = ut_table_alloc(main_thread);
  if (tid != 0) {
    free(main_thread);
    return;
  }

  main_thread->tid = 0;
  main_thread->stack.size = UTHREAD_STACK_SIZE;
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
  new_thread->func.start = func;
  new_thread->func.arg = malloc(nbytes);
  if (!new_thread->func.arg) {
    ut_table_release(tid);
    free(new_thread);
    return FAILURE;
  }

  memcpy(new_thread->func.arg, args, nbytes);

  // Stack Initialization
  new_thread->stack.size = UTHREAD_STACK_SIZE;
  new_thread->stack.base = malloc(new_thread->stack.size);
  if (!new_thread->stack.base) {
    free(new_thread->func.arg);
    ut_table_release(tid);
    free(new_thread);
    return FAILURE;
  }

  uintptr_t sp = (uintptr_t)new_thread->stack.base + new_thread->stack.size;

  sp &= ~(uintptr_t)0xF;

  sp -= sizeof(uint64_t);
  *(uint64_t *)sp = (uint64_t)func;

  // Context Initialization
  new_thread->context = (uctx_t){0};

  new_thread->context.rsp = sp;
  new_thread->context.rdi = (uint64_t)new_thread->func.arg;

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
