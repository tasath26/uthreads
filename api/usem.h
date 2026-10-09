/*  -----------------------------------------------------
 *    CS527 - Parallel Computer Architecture - Assign 1
 *              Tasos Athanasiadis - csdp1440
 *
 *  =====================================================
 *                         usem.h
 *  =====================================================
 *
 *            Public API for semaphore library 
 *
 *  -----------------------------------------------------
 */

#ifndef USEM_H
#define USEM_H

typedef struct usem{
    long count;
    void *queue;
}usem_t;

void usem_init(usem_t *s, long count);

void usem_wait(usem_t *s);

void usem_signal(usem_t *s);

#endif
