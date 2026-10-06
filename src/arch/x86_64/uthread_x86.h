#ifndef UTHREAD_X86_H
#define UTHREAD_X86_H

#include <stddef.h>
#include <stdint.h>

typedef struct uctx {
  uint64_t rsp;
  uint64_t rbx;
  uint64_t rbp;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t rdi;
} uctx_t;

extern void _uthread_start(void);

#endif // !UTHREAD_X86_H
