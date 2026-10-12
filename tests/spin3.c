#include <stdio.h>
#include <stdlib.h>
#include "uthreads.h"
#include "usem.h"

usem_t mutex;
int y;

struct args {
        usem_t *mutex;
        int *ip;
};

void incr(struct args *p) {
        int i;
        static int j;

        for (i = 0; i < 10000; i++) {
                usem_wait(p->mutex);
                *p->ip = *p->ip + 1;
                usem_signal(p->mutex);
        }
        printf("%d:(%d:y)=%d\n", j, uthread_self(), y);
        j++;
}

int main(int argc, char *argv[]) {
        int i, j;
        int m = argc > 1 ? atoi(argv[1]) : 4;
        int n = argc > 2 ? atoi(argv[2]) : 3;

        uthread_init();
        usem_init(&mutex, 1);
        for (j = 0; j < n; j++) {
                struct args args;
                args.mutex = &mutex;
                args.ip = &y;
                for (i = 0; i < m; i++)
                        uthread_new((int (*)(void *))incr, &args, sizeof args, NULL);
                uthread_join(0);
                if (y != m*10000)
                        printf("%d:%d\n", j, y);
                y = 0;
        }
        uthread_exit(0);
        return 0;
}
