#include <stdint.h>
#include <stddef.h>
#include "uthread_arm32.h"


void arch_context_init(
    uctx_t *ctx,
    void *stack_base,
    size_t stack_size,
    int (*func)(void *),
    void *arg
)
{
    uintptr_t sp = (uintptr_t)stack_base + stack_size;
    sp &= ~(uintptr_t)0x7;

    /*
     * A terminating frame record:
     * [0] = previous frame pointer
     * [1] = previous return address
     */
    sp -= 2 * sizeof(uint32_t);
    uint32_t *frame = (uint32_t *)sp;
    frame[0] = 0;
    frame[1] = 0;

    *ctx = (uctx_t){0};

    ctx->sp  = (uint32_t)sp;
    ctx->r11 = (uint32_t)sp;
    ctx->lr  = (uint32_t)(uintptr_t)_uthread_start;
    ctx->r4  = (uint32_t)(uintptr_t)func;
    ctx->r5  = (uint32_t)(uintptr_t)arg;

    /*
     * r9 may have a platform-specific role.
     * Preserve the creator's value for the new context.
     */
    __asm__ volatile("mov %0, r9" : "=r"(ctx->r9));
}
