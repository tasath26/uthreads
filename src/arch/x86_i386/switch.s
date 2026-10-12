.text
.p2align 2
.globl __uthread_switch
.type __uthread_switch, @function

__uthread_switch:
    # Stack arguments:
    # 4(%esp) = old context
    # 8(%esp) = new context

    movl 4(%esp), %eax
    movl 8(%esp), %edx

    # Save old context.
    movl %esp,  0(%eax)
    movl %ebx,  4(%eax)
    movl %esi,  8(%eax)
    movl %edi, 12(%eax)
    movl %ebp, 16(%eax)

    # Save x87/MMX/SSE state.
    fxsave 32(%eax)

    # Restore new context's floating-point state.
    fxrstor 32(%edx)

    # Restore new context.
    movl  4(%edx), %ebx
    movl  8(%edx), %esi
    movl 12(%edx), %edi
    movl 16(%edx), %ebp
    movl  0(%edx), %esp

    ret

.size __uthread_switch, .-__uthread_switch
