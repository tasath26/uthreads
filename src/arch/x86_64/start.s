.text
.globl _uthread_start
.type _uthread_start, @function

_uthread_start:
    movq %r13, %rdi
    call *%r12    

    movl %eax, %edi        
    call uthread_exit
    ud2                    

