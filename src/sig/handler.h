#ifndef SIG_HANDLER_H
#define SIG_HANDLER_H

#include <signal.h>

void sig_init(void);
void sig_start(void);
void sig_stop(void);

void sig_disable(sigset_t *prev);
void sig_restore(const sigset_t *prev);

void sig_enable(void);

#define ENTER_CRITICAL(saved) \
    sigset_t saved;           \
    sig_disable(&(saved))

#define EXIT_CRITICAL(saved) \
    sig_restore(&(saved))


#endif // !HANDLER_H
