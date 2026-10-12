#include <stddef.h>
#include <stdint.h>

#include "uthread_rv64.h"

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
     * Terminating frame record:
     * [0] = previous frame pointer
     * [1] = previous return address
     *
     * On RISC-V, the frame pointer points ABOVE
     * the two saved words.
     */
    sp -= 2 * sizeof(uintptr_t);

    uintptr_t *frame = (uintptr_t *)sp;
    frame[0] = 0;
    frame[1] = 0;

    *ctx = (uctx_t){0};

    ctx->sp = sp;
    ctx->ra = (uintptr_t)_uthread_start;

    // Function and argument
    ctx->s1 = (uintptr_t)func;
    ctx->s2 = (uintptr_t)arg;

    // Conventional frame pointer (CFA)
    ctx->s0 = sp + 2 * sizeof(uintptr_t);

    // Inherit floating-point environment
    uint32_t fcsr;
    __asm__ volatile("frcsr %0" : "=r"(fcsr));

    ctx->fcsr = fcsr;
}
