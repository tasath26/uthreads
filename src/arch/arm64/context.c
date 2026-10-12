#include "uthread_arm64.h"


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

    /*
     * A terminating frame record:
     * [0] = previous frame pointer
     * [1] = previous return address
     */
    sp -= 2 * sizeof(uint64_t);
    uint64_t *frame = (uint64_t *)sp;
    frame[0] = 0;
    frame[1] = 0;

    *ctx = (uctx_t){0};

    ctx->sp  = sp;
    ctx->x29 = sp;
    ctx->x30 = (uintptr_t)_uthread_start;
    ctx->x19 = (uintptr_t)func;
    ctx->x20 = (uintptr_t)arg;

    // Inherit the creator's floating-point environment.
    uint64_t fpcr, fpsr;
    __asm__ volatile("mrs %0, fpcr" : "=r"(fpcr));
    __asm__ volatile("mrs %0, fpsr" : "=r"(fpsr));

    ctx->fpcr = (uint32_t)fpcr;
    ctx->fpsr = (uint32_t)fpsr;
}
