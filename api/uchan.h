/*  -----------------------------------------------------
 *    CS527 - Parallel Computer Architecture - Assign 1
 *              Tasos Athanasiadis - csdp1440
 *
 *  =====================================================
 *                         uchan.h
 *  =====================================================
 *
 *            Public API for channel library 
 *
 *  -----------------------------------------------------
 */

#ifndef UCHAN_H
#define UCHAN_H

#include <stddef.h>

typedef struct uchan *uchan_t;

uchan_t uchan_new(void);

size_t uchan_send(uchan_t c, void *ptr, size_t size);

size_t uchan_receive(uchan_t c, void *ptr, size_t size);

#endif
