// x86_64 Context initialization

#include "uthread_x86.h"

void arch_context_init(
    uctx_t *ctx,
    void *stack_base,
    size_t stack_size,
    int (*func)(void *),
    void *arg
)
{
    uintptr_t sp = (uintptr_t)stack_base + stack_size;
    sp &= ~(uintptr_t)0xF;

    sp -= sizeof(uint64_t);
    *(uint64_t *)sp = (uint64_t)(uintptr_t)_uthread_start;

    *ctx = (uctx_t){0};

    ctx->rsp = sp; 
    ctx->r12 = (uint64_t)(uintptr_t)func;
    ctx->r13 = (uint64_t)(uintptr_t)arg;
}
