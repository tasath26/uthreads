
#include <assert.h>
#include "uthreads.h"

static int finished = 0;

static int worker(void *arg)
{
    (void)arg;
    uthread_pause();
    finished = 1;
    return 42;
}

int main(void)
{
    uthread_init();

    int tid = uthread_new(worker, NULL, 0, NULL);
    assert(tid > 0);

    uthread_join(tid);
    assert(finished == 1);

    return 0;
}

