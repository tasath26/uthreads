.text
.p2align 2
.globl __uthread_start

__uthread_start:
    bl  _sig_enable
    mov x0, x20          // First argument: arg
    blr x19              // Call func(arg)

    bl _uthread_exit     // C function: uthread_exit(code)

Lunexpected_return:
    brk #0
    b Lunexpected_return
