.text
.p2align 2
.globl __uthread_start
.type __uthread_start, @function

__uthread_start:
    call _sig_enable

    mv a0, s2              # First argument: arg
    jalr ra, 0(s1)         # Call func(arg)

    call _uthread_exit     # uthread_exit(return_value)

Lunexpected_return:
    ebreak
    j Lunexpected_return

.size __uthread_start, .-__uthread_start
