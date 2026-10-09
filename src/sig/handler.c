#include "handler.h"

static sigset_t set;
static volatile sig_atomic_t tick = 0;
