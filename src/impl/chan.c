#include "uchan.h"
#include "usem.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct uchan{
  void *ptr;
  size_t *size;
  usem_t send;
  usem_t rec; 
  usem_t sync;
};

uchan_t uchan_new(void){
  uchan_t ch = calloc(1, sizeof(*ch));
  if(!ch){
    abort();
  }

  usem_init(&ch->send, 1);
  usem_init(&ch->rec, 0);
  usem_init(&ch->sync, 0);

  return ch;
}

size_t uchan_send(uchan_t ch, void *ptr, size_t size){
  assert(ptr);
  assert(ch);

  usem_wait(&ch->send);
  ch->ptr = ptr;
  ch->size = &size;
  usem_signal(&ch->sync);
  
  usem_wait(&ch->sync);

  return size;
}

size_t uchan_receive(uchan_t ch, void *ptr, size_t size){
  assert(ch);
  assert(ptr);

  size_t n = 0;
  long *src = NULL;
  
  usem_wait(&ch->rec);
  
  n = *ch->size;
  if(size < n){
    n = size;
  }
  *ch->size = n;

  if(n > 0){
    memcpy(ptr, ch->ptr, n);
  }

  usem_signal(&ch->send);
  usem_signal(&ch->sync);

  return n;
}
