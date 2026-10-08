#ifndef UTHREAD_ARM64_H
#define UTHREAD_ARM64_H

#include <stddef.h>
#include <stdint.h>


typedef struct uctx {
    uint64_t sp;         /*   0 */
    uint64_t x30;        /*   8: return address */

    uint64_t x19;        /*  16 */
    uint64_t x20;        /*  24 */
    uint64_t x21;        /*  32 */
    uint64_t x22;        /*  40 */
    uint64_t x23;        /*  48 */
    uint64_t x24;        /*  56 */
    uint64_t x25;        /*  64 */
    uint64_t x26;        /*  72 */
    uint64_t x27;        /*  80 */
    uint64_t x28;        /*  88 */
    uint64_t x29;        /*  96: frame pointer */

    uint64_t d8;         /* 104: raw register bits */
    uint64_t d9;         /* 112 */
    uint64_t d10;        /* 120 */
    uint64_t d11;        /* 128 */
    uint64_t d12;        /* 136 */
    uint64_t d13;        /* 144 */
    uint64_t d14;        /* 152 */
    uint64_t d15;        /* 160 */

    uint32_t fpcr;       /* 168: floating-point control */
    uint32_t fpsr;       /* 172: floating-point status */
} uctx_t;


extern void _uthread_start(void);

#endif 
