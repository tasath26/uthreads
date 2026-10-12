#include <fcntl.h>
#include <poll.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../sig/handler.h"

extern ssize_t __real_read(int fd, void *buf, size_t count);
extern ssize_t __real_write(int fd, const void *buf, size_t count);

ssize_t __wrap_read(int fd, void *buf, size_t count)
{
    ssize_t result = -1;
    ENTER_CRITICAL(saved);
    result = __real_read(fd, buf, count);
    EXIT_CRITICAL(saved);
    return result;
}


ssize_t __wrap_write(int fd, const void *buf, size_t count)
{
    ssize_t result = -1;
    ENTER_CRITICAL(saved);
    result = __real_write(fd, buf, count);
    EXIT_CRITICAL(saved);
    return result;
}
