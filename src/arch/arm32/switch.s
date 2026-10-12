.syntax unified
.arm
.text
.p2align 2
.globl __uthread_switch
.type __uthread_switch, %function

__uthread_switch:
    @ r0 = old context, r1 = new context

    @ Save old context.
    stmia r0, {r4-r11}
    str sp, [r0, #32]
    str lr, [r0, #36]

    add r2, r0, #40
    vstmia r2, {d8-d15}

    vmrs r2, fpscr
    str r2, [r0, #104]

    @ Restore new context.
    ldmia r1, {r4-r11}

    add r2, r1, #40
    vldmia r2, {d8-d15}

    ldr r2, [r1, #104]
    vmsr fpscr, r2

    ldr lr, [r1, #36]
    ldr r2, [r1, #32]
    mov sp, r2

    bx lr

.size __uthread_switch, .-__uthread_switch
