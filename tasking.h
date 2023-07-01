#include <st.h>
typedef void* (*cfunc)();

typedef struct args
{
   void in[1];
   char type;
} arg_t;

typedef struct ompc_task
{
   cfunc func;
   arg_t *args
} ompc_task_t;

void _ompc_task_create(cfunc func, arg_t args);
void _ompc_task_start(int a, int b, struct ompc_task *tp, st_join_counter_t *cc);
void _ompc_task_wait(int num_task, struct ompc_task *tp);