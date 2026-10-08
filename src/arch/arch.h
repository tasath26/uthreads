#ifndef ARCH_H
#define ARCH_H

#if defined(X86_64)
  #include "x86_64/uthread_x86.h"
#elif defined(RISCV)
  #include "risc-v/uthread_rv64.h"
#elif defined(ARM64)
  #include "arm64/uthread_arm64.h"
#endif

void arch_context_init(
    uctx_t *ctx,
    void *stack_base,
    size_t stack_size,
    int (*func)(void *),
    void *arg
);
  
extern void _uthread_switch(uctx_t *from, uctx_t *to);

#endif // !ARCH_H
