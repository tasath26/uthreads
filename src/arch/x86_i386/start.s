.text
.p2align 2
.globl __uthread_start
.type __uthread_start, @function

__uthread_start:
    call _sig_enable

    # func(arg)
    subl $16, %esp
    movl %esi, (%esp)
    call *%ebx
    addl $16, %esp

    # uthread_exit(return_value)
    subl $16, %esp
    movl %eax, (%esp)
    call _uthread_exit

Lunexpected_return:
    ud2
    jmp Lunexpected_return

.size __uthread_start, .-__uthread_start
