.syntax unified
.text
.p2align 2
.globl __uthread_start
.type __uthread_start, %function

__uthread_start:
    bl  _sig_enable
    mov r0, r5           
    blx r4               

    bl  _uthread_exit    

Lunexpected_return:
    udf #0
    b Lunexpected_return
