#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include "../sig/handler.h"

extern int __real_vprintf(const char *format, va_list args);
extern int __real_vfprintf(FILE *stream, const char *format, va_list args);
extern int __real_puts(const char *s);
extern int __real_fputs(const char *s, FILE *stream);
extern int __real_putchar(int c);
extern int __real_fputc(int c, FILE *stream);
extern size_t __real_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern size_t __real_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
extern int __real_fflush(FILE *stream);
extern FILE * __real_fopen(const char *path, const char *mode);
extern int __real_fclose(FILE *stream);

int __wrap_printf(const char *format, ...)
{
    int result;
    int saved_errno;
    va_list args;
    va_start(args, format);

    ENTER_CRITICAL(saved);
    result = __real_vprintf(format, args);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    va_end(args);
    errno = saved_errno;
    return result;
}

int __wrap_fprintf(FILE *stream, const char *format, ...)
{
    int result;
    int saved_errno;
    va_list args;
    va_start(args, format);

    ENTER_CRITICAL(saved);
    result = __real_vfprintf(stream, format, args);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    va_end(args);
    errno = saved_errno;
    return result;
}

int __wrap_vprintf(const char *format, va_list args)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_vprintf(format, args);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_vfprintf(FILE *stream, const char *format, va_list args)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_vfprintf(stream, format, args);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_puts(const char *s)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_puts(s);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_fputs(const char *s, FILE *stream)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fputs(s, stream);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_putchar(int c)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_putchar(c);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_fputc(int c, FILE *stream)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fputc(c, stream);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

size_t __wrap_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fread(ptr, size, nmemb, stream);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

size_t __wrap_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fwrite(ptr, size, nmemb, stream);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_fflush(FILE *stream)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fflush(stream);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

FILE * __wrap_fopen(const char *path, const char *mode)
{
    FILE * result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fopen(path, mode);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}

int __wrap_fclose(FILE *stream)
{
    int result;
    int saved_errno;

    ENTER_CRITICAL(saved);
    result = __real_fclose(stream);
    saved_errno = errno;
    EXIT_CRITICAL(saved);

    errno = saved_errno;
    return result;
}
