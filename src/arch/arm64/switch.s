.text
.p2align 2
.globl __uthread_switch

__uthread_switch:
    // x0 = old context, x1 = new context

    // Save old context.
    mov x9, sp
    str x9,  [x0, #0]
    str x30, [x0, #8]

    stp x19, x20, [x0, #16]
    stp x21, x22, [x0, #32]
    stp x23, x24, [x0, #48]
    stp x25, x26, [x0, #64]
    stp x27, x28, [x0, #80]
    str x29,      [x0, #96]

    stp d8,  d9,  [x0, #104]
    stp d10, d11, [x0, #120]
    stp d12, d13, [x0, #136]
    stp d14, d15, [x0, #152]

    mrs x9, fpcr
    str w9, [x0, #168]
    mrs x9, fpsr
    str w9, [x0, #172]

    // Restore new context.
    ldp x19, x20, [x1, #16]
    ldp x21, x22, [x1, #32]
    ldp x23, x24, [x1, #48]
    ldp x25, x26, [x1, #64]
    ldp x27, x28, [x1, #80]
    ldr x29,      [x1, #96]
    ldr x30,      [x1, #8]

    ldp d8,  d9,  [x1, #104]
    ldp d10, d11, [x1, #120]
    ldp d12, d13, [x1, #136]
    ldp d14, d15, [x1, #152]

    ldr w9, [x1, #168]
    msr fpcr, x9
    ldr w9, [x1, #172]
    msr fpsr, x9

    ldr x9, [x1, #0]
    mov sp, x9

    ret
