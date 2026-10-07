#include "table.h"
#include "../common.h"

#define INIT_TABLE_CAP 16

int table_cap = 0;
void **table = NULL;

int ut_table_init(void){
  table = calloc(INIT_TABLE_CAP, sizeof(*table));

  if (table == NULL)
      return FAILURE;

  table_cap = INIT_TABLE_CAP;

  return 0;
}

static int ut_table_grow(){
  size_t old_cap = table_cap;
  size_t new_cap = old_cap * 2;

  void **new_table = realloc(table, new_cap * sizeof(*new_table));

  if (new_table == NULL)
        return -1;

  memset(new_table + old_cap, 0, (new_cap - old_cap) * sizeof(*new_table) );

  table = new_table;
  table_cap = new_cap;

  return 0;
}

int ut_table_alloc(uthread_t *thread)
{
    for (;;) {
        for (size_t i = 0; i < table_cap; i++) {
            if (table[i] == NULL) {
                table[i] = thread;
                return (int)i;
            }
        }

        if (ut_table_grow() != 0)
            return -1;
    }
}

uthread_t * ut_table_lookup(int tid){

  if (tid < 0 || (size_t)tid >= table_cap)
      return NULL;

  return table[tid];

}

void ut_table_release(int tid)
{
    if (tid < 0 || tid >= table_cap) {
        return;
    }

    table[tid] = NULL;
}
