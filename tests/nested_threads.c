
#include <assert.h>
#include "uthreads.h"

#define DEPTH 100

static int completed = 0;

static int worker(void *arg)
{
    int depth = *(int *)arg;

    if (depth > 0) {
        int next = depth - 1;
        int tid = uthread_new(worker, &next, sizeof(next), NULL);
        assert(tid > 0);
        uthread_join(tid);
    }

    completed++;
    return 0;
}

int main(void)
{
    uthread_init();

    int depth = DEPTH;
    int tid = uthread_new(worker, &depth, sizeof(depth), NULL);

    uthread_join(tid);

    assert(completed == DEPTH + 1);
    return 0;
}

