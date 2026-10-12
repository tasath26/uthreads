#include <stdio.h>
#include <stdlib.h>
#include "uthreads.h"

int n;

int f(void *argp) {
        int *p = argp;

        while ((*p)--) {  n = n + 1; }
        return 0;
}

int main(int argc, char *argv[]) {
        int i, m = 5;
        unsigned count = 10000;

        uthread_init();
        if (argc >= 2)
                m = atoi(argv[1]);
        for (i = 0; i < m; i++)
                uthread_new(f, &count, sizeof count, NULL);
        uthread_join(0);
        printf("%d == %d\n", n, m*count);
        uthread_exit(0);
        return 0;
}
