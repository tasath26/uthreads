#ifndef UTHREAD_RV64_H
#define UTHREAD_RV64_H

#include <stddef.h>
#include <stdint.h>

typedef struct uctx {
    uintptr_t sp;
    uintptr_t ra;

    uintptr_t s0;
    uintptr_t s1;
    uintptr_t s2;
    uintptr_t s3;
    uintptr_t s4;
    uintptr_t s5;
    uintptr_t s6;
    uintptr_t s7;
    uintptr_t s8;
    uintptr_t s9;
    uintptr_t s10;
    uintptr_t s11;

    // Callee-saved floating-point registers
    uint64_t fs0_fs11[12];

    // Floating-point control and status
    uint32_t fcsr;
} uctx_t;

extern void _uthread_start(void);

#endif 
