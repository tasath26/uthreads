#include <stdio.h>
#include "uthreads.h"

static int worker(void *arg)
{
    (void)arg;

    printf("Hello from thread %d\n", uthread_self());

    for(size_t i = 0; i < 100000; i = i + 2){
      i--;
    }
    return 0;
}

int main(void)
{
    uthread_init();

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
