#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdlib.h>
#include <string.h>
#include "../common.h"

int ut_table_init(void);

int ut_table_alloc(uthread_t* thread);

uthread_t * ut_table_lookup(int tid);

void ut_table_release(int tid);

#endif // !_TABLE_H_
