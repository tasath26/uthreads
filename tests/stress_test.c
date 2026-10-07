#include "uthreads.h"
#include <stdio.h>
#include <stdlib.h>

#define CHECK(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #expr); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

enum { WORKERS = 64, ROUNDS = 50, JOINERS = 32, CYCLES = 1000 };
static int main_tid;
static int progress[WORKERS];
static int allow_exit[WORKERS];

static int create(int (*func)(void *), void *arg, size_t size)
{
    int tid = uthread_new(func, arg, size);
    CHECK(tid > 0);
    return tid;
}

static int round_worker(void *arg)
{
    int index = *(int *)arg;

    fprintf(stderr, "arg=%p, index=%d\n", arg, index);
    CHECK(index >= 0 && index < WORKERS);
    int self = uthread_self();
    int local = 0;

    for (int r = 0; r < ROUNDS; ++r) {
        CHECK(uthread_self() == self);
        CHECK(local == r);
        ++local;
        ++progress[index];
        uthread_pause();
        CHECK(uthread_self() == self);
    }

    /* Keep each target alive until main joins it. */
    while (!allow_exit[index]) {
        uthread_pause();
    }
    CHECK(local == ROUNDS);
    return 100 + index;
}

static void test_rounds(void)
{
    int tids[WORKERS];
    for (int i = 0; i < WORKERS; ++i) {
        int arg = i;
        tids[i] = create(round_worker, &arg, sizeof(arg));
        arg = -999; /* A worker must receive the private copy. */
        for (int j = 0; j < i; ++j) {
            CHECK(tids[i] != tids[j]);
        }
    }

    for (int r = 0; r < ROUNDS; ++r) {
        uthread_pause();
        CHECK(uthread_self() == main_tid);
        for (int i = 0; i < WORKERS; ++i) {
            CHECK(progress[i] == r + 1);
        }
    }

    for (int i = 0; i < WORKERS; ++i) {
        allow_exit[i] = 1;
        int result = uthread_join(tids[i]);
        CHECK(result == 100 + i);
        CHECK(uthread_self() == main_tid);
        CHECK(uthread_join(tids[i]) == -1); /* Removed; no new ID reuse yet. */
    }
    puts("PASS: copied arguments, 64 workers, repeated yields and return codes");
}

static int entered;
static int finished;
static int target_finished;
static int target_tid;

static int shared_target(void *arg)
{
    (void)arg;
    int self = uthread_self();
    while (entered != JOINERS) {
        uthread_pause();
        CHECK(uthread_self() == self);
    }
    /* Every joiner increments entered and immediately calls join,
       so all are blocked before this cooperative target resumes. */
    for (int i = 0; i < 3; ++i) {
        CHECK(finished == 0);
        uthread_pause();
    }
    target_finished = 1;
    uthread_exit(73);
    CHECK(0); /* exit must never return */
    return 0;
}

static int joiner(void *arg)
{
    (void)arg;
    int self = uthread_self();
    ++entered;
    int result = uthread_join(target_tid);
    CHECK(target_finished == 1);
    CHECK(result == 73);
    CHECK(uthread_self() == self);
    ++finished;
    return 0;
}

static void test_multiple_joiners(void)
{
    entered = finished = target_finished = 0;
    target_tid = create(shared_target, NULL, 0);
    for (int i = 0; i < JOINERS; ++i) {
        create(joiner, NULL, 0);
    }
    while (finished != JOINERS) {
        uthread_pause();
        CHECK(uthread_self() == main_tid);
    }
    CHECK(uthread_join(target_tid) == -1);
    puts("PASS: 32 joiners wake on exit, not on target yield");
}

static int cycle_worker(void *arg)
{
    int value = *(int *)arg;
    int self = uthread_self();
    uthread_pause();
    CHECK(uthread_self() == self);
    if ((value & 1) != 0) {
        uthread_exit(value);
        CHECK(0);
    }
    return value;
}

static void test_cycles(void)
{
    for (int i = 0; i < CYCLES; ++i) {
        int tid = create(cycle_worker, &i, sizeof(i));
        int result = uthread_join(tid);
        CHECK(result == i);
        CHECK(uthread_self() == main_tid);
        CHECK(uthread_join(tid) == -1);
    }
    puts("PASS: 1000 create/join/exit cycles (explicit exit and return)");
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    uthread_init();
    main_tid = uthread_self();
    CHECK(main_tid == 0);

    CHECK(uthread_join(-1) == -1);
    CHECK(uthread_join(1) == -1); /* No worker exists yet. */
    for (int i = 0; i < 10; ++i) {
        uthread_pause();
        CHECK(uthread_self() == main_tid);
    }
    puts("PASS: nonexistent targets and pause with only main runnable");

    test_rounds();
    test_multiple_joiners();
    test_cycles();

    uthread_pause();
    CHECK(uthread_self() == main_tid);
    puts("ALL TESTS PASSED");
    return 0;
}
