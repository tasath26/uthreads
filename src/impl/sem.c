#include "../arch/arch.h"
#include "../common/common.h"
#include "../common/structures/queue.h"

#include "usem.h"

#include <assert.h>

void usem_init(usem_t *s, long count){

  assert(s != NULL);
  ut_queue_t *new_queue = malloc(sizeof(*new_queue));
  if(!new_queue)
    abort();

  if(ut_queue_init(new_queue) == -1){
    free(new_queue);
    abort();
  }

  s->queue = (void*)new_queue;
  s->count = count;

  return;
}

void usem_wait(usem_t *s){

  if(s->count <= 0){   
    ut_queue_enq(s->queue, current_thread);    
    ut_queue_deq(&thread_queue);

    if(ut_queue_empty(&thread_queue)){
      abort();
    }

    uthread_switch(); 

  }else{
    s->count--;
  }

}

void usem_signal(usem_t *s){
  
  if(s->count > 0 && !ut_queue_empty(s->queue)){

    uthread_t *waiting = ut_queue_deq(s->queue);
    ut_queue_enq(&thread_queue, waiting);

  }else{
    s->count++;
  }

}
