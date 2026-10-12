#include <assert.h>
#include "uthreads.h"

static volatile int observed = 0;

static int worker(void *arg)
{
    (void)arg;
    observed = 1;
    return 0;
}

int main(void)
{
    uthread_init();

    uthread_new(worker, NULL, 0, NULL);

    // No voluntary yielding here.
    while (!observed) {
    }

    assert(observed == 1);
    return 0;
}
