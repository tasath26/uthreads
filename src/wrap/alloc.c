#include <stdlib.h>
#include "../sig/handler.h"

extern void *__real_malloc(size_t size);
extern void *__real_calloc(size_t nmemb, size_t size);
extern void *__real_realloc(void *ptr, size_t size);
extern void __real_free(void *ptr);

void *__wrap_malloc(size_t size)
{
  ENTER_CRITICAL(prev);
  void *ptr = __real_malloc(size);
  EXIT_CRITICAL(prev);

  return ptr;
}

void *__wrap_calloc(size_t nmemb, size_t size)
{
  ENTER_CRITICAL(saved);
  void *ptr = __real_calloc(nmemb, size);
  EXIT_CRITICAL(saved);

  return ptr;
}

void *__wrap_realloc(void *ptr, size_t size)
{
  ENTER_CRITICAL(saved);
  ptr = __real_realloc(ptr, size);
  EXIT_CRITICAL(saved);

  return ptr;
}

void __wrap_free(void *ptr)
{
  ENTER_CRITICAL(saved);
  __real_free(ptr);
  EXIT_CRITICAL(saved);
}
