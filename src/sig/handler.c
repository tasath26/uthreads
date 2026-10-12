#include "../arch/arch.h"
#include "../common/common.h"
#include "../common/structures/queue.h"
#include "handler.h"

#include <stdlib.h>
#include <sys/time.h>

#define PREEMPT_SIGNAL SIGVTALRM
#define UTHREAD_PRMT_INTERVAL_US 10000

static volatile sig_atomic_t tick = 0;
static sigset_t preempt_set;

static void uthread_preempt_tick(void)
{
    /* SIGVTALRM is blocked while executing this handler path. */
    if (thread_queue.count <= 1) {
        return;
    }

    uthread_t *from = current_thread;

    ut_queue_deq(&thread_queue);

    if (ut_queue_enq(&thread_queue, from) == -1) {
        abort();
    }

    uthread_switch();
}


static void timer_handler(int signo)
{
    (void)signo;
    uthread_preempt_tick();
}

void sig_init(void)
{
    if (sigemptyset(&preempt_set) == -1 ||
        sigaddset(&preempt_set, PREEMPT_SIGNAL) == -1) {
        abort();
    }

    struct sigaction action = {0};

    action.sa_handler = timer_handler;

    /* Keep timer interrupts blocked while handling a tick. */
    if (sigemptyset(&action.sa_mask) == -1 ||
        sigaddset(&action.sa_mask, PREEMPT_SIGNAL) == -1) {
        abort();
    }

    action.sa_flags = 0;

    if (sigaction(PREEMPT_SIGNAL, &action, NULL) == -1) {
        abort();
    }
}

void sig_start(void)
{
    struct itimerval timer = {
        .it_interval = {
            .tv_sec = 0,
            .tv_usec = UTHREAD_PRMT_INTERVAL_US
        },
        .it_value = {
            .tv_sec = 0,
            .tv_usec = UTHREAD_PRMT_INTERVAL_US 
        }
    };

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        abort();
    }
}

void sig_stop(void)
{
    struct itimerval timer = {0};

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        abort();
    }
}

void sig_disable(sigset_t *prev)
{
    if (sigprocmask(SIG_BLOCK, &preempt_set, prev) == -1) {
        abort();
    }
}

void sig_restore(const sigset_t *prev)
{
    if (sigprocmask(SIG_SETMASK, prev, NULL) == -1) {
        abort();
    }
}

void sig_enable(void)
{
    if (sigprocmask(SIG_UNBLOCK, &preempt_set, NULL) == -1) {
        abort();
    }
}
