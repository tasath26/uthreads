#include <stdlib.h>
#include "../common.h"
#include "queue.h"

#define UT_QUEUE_INITIAL_CAPACITY 16
#define SUCCESS 0

int ut_queue_init(ut_queue_t *q)
{
    q->thread = malloc(UT_QUEUE_INITIAL_CAPACITY * sizeof(*q->thread));
    if (!q->thread)
        return FAILURE;

    q->capacity = UT_QUEUE_INITIAL_CAPACITY;
    q->head = 0;
    q->tail = 0;
    q->count = 0;

    return SUCCESS;
}


int ut_queue_enq(ut_queue_t *q, uthread_t *thread)
{
    if (q->tail == q->capacity && q->head > 0) {
        for (size_t i = 0; i < q->count; i++)
            q->thread[i] = q->thread[q->head + i];

        q->head = 0;
        q->tail = q->count;
    }

    if (q->tail == q->capacity) {
        size_t new_capacity = q->capacity * 2;

        uthread_t **new_thread =
            realloc(q->thread, new_capacity * sizeof(*q->thread));

        if (!new_thread)
            return FAILURE;

        q->thread = new_thread;
        q->capacity = new_capacity;
    }

    q->thread[q->tail++] = thread;
    q->count++;

    return SUCCESS;
}


uthread_t *ut_queue_deq(ut_queue_t *q)
{
    if (q->count == 0)
        return NULL;

    uthread_t *thread = q->thread[q->head++];

    q->count--;

    if (q->count == 0) {
        q->head = 0;
        q->tail = 0;
    }

    return thread;
}


void ut_queue_destroy(ut_queue_t *q)
{
    free(q->thread);

    q->thread = NULL;
    q->capacity = 0;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}


int ut_queue_empty(const ut_queue_t *q)
{
    return q->count == 0;
}

uthread_t *ut_queue_peek(ut_queue_t *q)
{
  return q->thread[q->head];
}
