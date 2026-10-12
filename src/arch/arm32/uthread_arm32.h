#ifndef UTHREAD_ARM32_H
#define UTHREAD_ARM32_H

#include <stdint.h>

typedef struct uctx {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;

    uint32_t sp;
    uint32_t lr;

    // VFP/NEON callee-saved registers
    uint64_t d8_d15[8];
} uctx_t;


extern void _uthread_start(void);

#endif
