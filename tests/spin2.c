#include <stdio.h>
#include <stdlib.h>
#include "uthreads.h"
#include "usem.h"

usem_t mutex;
int y = 0;

struct args { usem_t *mutex; int *ip; };

void incr(struct args *p) {
        int i;

        for (i = 0; i < 10000; i++) {
                usem_wait(p->mutex);
                *p->ip = *p->ip + 1;
                usem_signal(p->mutex);
        }
}

int main(int argc, char *argv[]) {
        int i, m = argc > 1 ? atoi(argv[1]) : 5;
        static struct args args = { &mutex, &y };

        uthread_init();
        usem_init(&mutex, 1);
        for (i = 0; i < m; i++)
                uthread_new((int (*)(void *))incr, &args, sizeof args, NULL);
        uthread_join(0);
        printf("%d = %d\n", y, m*10000);
        uthread_exit(0);
        return 0;
}
