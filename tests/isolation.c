#include <assert.h>
#include <stdint.h>
#include "uthreads.h"

#define NTHREADS 100

static int worker(void *arg)
{
    uintptr_t id = *(uintptr_t *)arg;
    volatile uint64_t values[128];

    for (int i = 0; i < 128; i++)
        values[i] = id;

    for (int n = 0; n < 100; n++) {
        uthread_pause();

        for (int i = 0; i < 128; i++)
            assert(values[i] == id);
    }

    return 0;
}

int main(void)
{
    uthread_init();

    int tids[NTHREADS];

    for (uintptr_t i = 0; i < NTHREADS; i++) {
        uintptr_t id = i;
        tids[i] = uthread_new(worker, &id, sizeof(id), NULL);
    }

    for (int i = 0; i < NTHREADS; i++)
        uthread_join(tids[i]);

    return 0;
}
