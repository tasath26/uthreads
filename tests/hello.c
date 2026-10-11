#include <stdio.h>
#include <stdlib.h>
#include "uthreads.h"
#include "usem.h"

usem_t sem;

int worker(void* arg){


  usem_wait(&sem);

  printf("Hello from thread %d\n", uthread_self());

  for(;;){}

  usem_signal(&sem);

  return 0;
}

int main(void)
{
    uthread_init();

    usem_init(&sem, 3);

    for (int i = 0; i < 1000; ++i) {
        if (uthread_new(worker, NULL, 0) == -1) {
            fprintf(stderr, "Failed to create thread\n");
            return 1;
        }
    }

    printf("Hello from main, thread %d\n", uthread_self());

    uthread_join(0);

    puts("Main resumed after all workers greeted.");
    return 0;
}
