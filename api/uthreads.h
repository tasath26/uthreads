/*  -----------------------------------------------------
 *    CS527 - Parallel Computer Architecture - Assign 1
 *              Tasos Athanasiadis - csdp1440
 *
 *  =====================================================
 *                         uthreads.h
 *  =====================================================
 *
 *       Public API for a User-space threads library
 *
 *  -----------------------------------------------------
 */

#ifndef UTHREADS_H
#define UTHREADS_H

#include <stddef.h>

/**
 * @func
 *    uthread_init
 *
 * @brief
 *    Initialize the user-level threading library.
 *    Must be called before any other thread function.
 */
void uthread_init(void);

/**
 * @func
 *    uthread_new
 *
 * @param func   Function executed by the new thread.
 * @param args   Pointer to argument data.
 * @param nbytes Size of the argument data in bytes.
 *
 * @return
 *    The new thread ID on success, or -1 on failure.
 *
 * @brief
 *    Create a new user-level thread.
 *    The new thread begins execution by calling func with a private copy
 *    of the nbytes bytes pointed to by args.
 */
int uthread_new(int func(void *), void *args, size_t nbytes);

/**
 * @func
 *    uthread_join
 *
 * @param tid  Thread descriptor ID.
 *
 * @return
 *    0 on success, or -1 on failure.
 *
 * @brief
 */
int uthread_join(int tid);

/**
 * @func
 *    uthread_self
 *
 * @return
 *    The ID of the current thread running.
 */
int uthread_self(void);

/**
 * @func
 *    uthread_pause
 *
 * @brief
 */
void uthread_pause(void);

#endif
