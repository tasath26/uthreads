#ifndef UTHREAD_X86_I386_H
#define UTHREAD_X86_I386_H

#include <stddef.h>
#include <stdint.h>

typedef struct uctx {
    uintptr_t esp;
    uintptr_t ebx;
    uintptr_t esi;
    uintptr_t edi;
    uintptr_t ebp;

    // Padding for 16-byte alignment
    uint8_t _padding[12];

    // x87, MMX, SSE and control/status state
    _Alignas(16) uint8_t fpstate[512];
} uctx_t;

extern void __uthread_start(void);

#endif
