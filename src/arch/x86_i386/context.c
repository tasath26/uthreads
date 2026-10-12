#include <stddef.h>
#include <stdint.h>

#include "uthread_x86_i386.h"

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
     * Synthetic return address.
     * Our context switch uses RET to enter the thread.
     */
    sp -= sizeof(uint32_t);
    *(uint32_t *)sp = (uint32_t)(uintptr_t)__uthread_start;

    *ctx = (uctx_t){0};

    ctx->esp = sp;
    ctx->ebp = 0;

    ctx->ebx = (uintptr_t)func;
    ctx->esi = (uintptr_t)arg;

    // Inherit creator's x87/SSE floating-point state.
    __asm__ volatile(
        "fxsave %0"
        : "=m"(ctx->fpstate)
        :
        : "memory"
    );
}
