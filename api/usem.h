#ifndef USEM_H
#define USEM_H

typedef struct usem *usem_t;

void usem_init(usem_t *s, long count);

void usem_wait(usem_t *s);

void usem_signal(usem_t *s);

#endif
