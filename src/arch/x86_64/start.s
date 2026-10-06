.text
.globl _thread_start
.type _thread_start, @function

_thrstart:
    subq $8, %rsp          
    call *%r12             
    movl %eax, %edi        
    call Thread_exit
    ud2                    

