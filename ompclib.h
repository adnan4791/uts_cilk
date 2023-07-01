/* 
 * $Id: ompclib.h,v 1.1.1.1 2004-02-06 18:15:04 msato Exp $
 * $RWC_Release$
 * $RWC_Copyright$
 */
/* RWC Omni OpenMP C compiler runtime lib. header */

#ifndef _OMPC_THREAD_H
#define _OMPC_THREAD_H
/*
#define USE_LOG 1
*/
#define TRUE 1
#define FALSE 0

//#include "exc_platform.h"
#include "ompc_reduction.h"
#include "st.h"

#ifdef USE_PTHREAD	/* pthread: set gcc include path to 2.6(pdph3) */
#include <pthread.h>
#include <sched.h>
typedef pthread_t _ompc_proc_t;
#ifdef not
#define _OMPC_WAIT(cond)	while(cond) sched_yield()
#else
#define MAX_COUNT 10000
#define _OMPC_WAIT(cond)\
{if(cond){ volatile int c = 0; while(cond){ if(c++>MAX_COUNT){ sched_yield(); c = 0; }}}}
#endif
#define _OMPC_PROC_SELF		pthread_self()
#endif

#ifdef USE_SOL_THREAD	/* solaris thread */
#ifndef _REENTRANT
#define _REENTRANT
#endif /* !_REENTRANT */
#include <sys/types.h>
#include <thread.h>
#include <synch.h>
#include <sys/processor.h>
#include <sys/procset.h>
typedef thread_t _ompc_proc_t;
#ifdef not
#define _OMPC_WAIT(cond)	while(cond) thr_yield()
#else
#define MAX_COUNT 10000
#define _OMPC_WAIT(cond)\
{if(cond){ volatile int c = 0; while(cond){ if(c++>MAX_COUNT){ thr_yield(); c = 0; }}}}
#endif
#define _OMPC_PROC_SELF		thr_self()
#endif

#if defined(USE_SPROC) && defined(OMNI_OS_IRIX)
#ifndef NO_RESOURCE_H
#include <sys/resource.h>
#endif /* !NO_RESOURCE_H */
#include <sys/prctl.h>
#include <signal.h>
#include <ulocks.h>
typedef pid_t _ompc_proc_t;
#ifdef not
#define _OMPC_WAIT(cond)        while(cond) sched_yield()
#else
#define MAX_COUNT 10000
#define _OMPC_WAIT(cond)\
{if(cond){ volatile int c = 0; while(cond){ if(c++>MAX_COUNT){ sched_yield(); c = 0; }}}}
#endif
#define _OMPC_PROC_SELF         getpid()
#endif /* USE_SPROC && OMNI_OS_IRIX */

#ifdef USE_SPIN_LOCK
# ifdef OMNI_OS_IRIX
typedef long PSM_mutex_t;
typedef PSM_mutex_t _ompc_lock_t;
# else
typedef int _ompc_lock_t;
# endif
#else
# ifdef USE_PTHREAD
typedef pthread_mutex_t _ompc_lock_t;
# endif
# ifdef USE_SOL_THREAD
typedef mutex_t _ompc_lock_t;
# endif
#endif

typedef struct {
  _ompc_lock_t	lock, wait;
  int		id;
  int		count;
} _ompc_nest_lock_t;

#define N_PROC_DEFAULT 4  	/* default */
#define MAX_PROC 256
#ifdef OMNI_CPU_MIPS
#define CACHE_LINE_SIZE	128
#else
#define CACHE_LINE_SIZE	32
#endif /* OMNI_CPU_MIPS */

#ifdef USE_SPROC
typedef void (*cfunc)();
#else
typedef void* (*cfunc)();
#endif

extern volatile int _ompc_nested;	/* nested enable/disable */
extern volatile int _ompc_dynamic;	/* dynamic enable/disable */
extern volatile int _ompc_max_threads;	/* max number of thread */
extern int _ompc_n_proc;

#define MAX_THDPRV 64
#define MAX_RED    64
#define RED_MASK   63

struct barrier_flag {
    volatile int _v;        /* for reduction */
    any_type r_v;           /* for reduction */
    struct barrier_flag *next;
};

struct ompc_parent {
    struct ompc_parent *parent;		/*  */
    struct ompc_thread *thread;
    int num_thds;	/* current running thread, refenced by children */
    cfunc func;
    void *args;

    volatile int dynamic_index;	  /* shared in children */

    volatile int ordered_id;	/* shared in team */
    volatile int ordered_lb;	/* shared in team */
    volatile int ordered_step;  /* shared in team */
    volatile int ordered_flag;  /* shared in team */
    struct ompc_context_list * ordered_context_list;

    /* sync for shared data, used for 'single' directive */
    /* shared by children */
    st_int_loc_t parent_count;	/* shared in team */

    /* structure for barrier in this team */
    st_join_counter_t *bc[2];	/* shared in team */
    struct barrier_flag *barrier_flags[MAX_RED];

    /* for synchronization */
    st_join_counter_t *jc;

    /* for nest lock */
    int ser_count;
};

struct ompc_task
{
  cfunc func;
  void *args;
  st_join_counter_t *mychild;
};

struct ompc_thread {
    struct ompc_parent *parent; /*  */
    int num;		/* the thread number of this thread in team */
    st_join_counter_t *mychild;

    /* used for schedule */
    int loop_chunk_size;
    int loop_end;
    int loop_sched_index;
    int loop_stride;		  /* used for static scheduling */

    /* used for 'sections' */
    int section_id; 
    int last_section_id;

    int loop_id;

    /* for threadprivate */
    void **thdprv[MAX_THDPRV];	/* shared in team */

    /* for 'lastprivate' */
    int is_last;

    /* sync for shared data */
    int count;

    /* structure for barrier */
    int barrier_phase;

    /* for reduction */
    int red_count;

    /* for nest lock */
    int ser_num;
};



struct ompc_context_list {
    struct ompc_context_list * next;
    struct st_context * c;
    int id;
};

/* library prototypes */
void _ompc_init(int argc,char *argv[]);
void _ompc_terminate(int);
void _ompc_init_proc_num(int);
void _ompc_do_parallel(cfunc f,void *args);
void _ompc_finalize(void);
void _ompc_fatal(char *msg);
void _ompc_current_thread_test();

void _ompc_ltc(cfunc func, void* args);
void _ompc_thread_fork(struct ompc_task *task, st_join_counter_t *cc);
void _ompc_thread_join();

void _ompc_init_lock(_ompc_lock_t *);
void _ompc_lock(volatile _ompc_lock_t *);
void _ompc_unlock(volatile _ompc_lock_t *);
void _ompc_destroy_lock(volatile _ompc_lock_t *);
int _ompc_test_lock(volatile _ompc_lock_t *);
void _ompc_init_nest_lock(_ompc_nest_lock_t *);
void _ompc_nest_lock(volatile _ompc_nest_lock_t *);
void _ompc_nest_unlock(volatile _ompc_nest_lock_t *);
void _ompc_destroy_nest_lock(volatile _ompc_nest_lock_t *);
int _ompc_test_nest_lock(volatile _ompc_nest_lock_t *);
void _ompc_thread_barrier(int i, struct ompc_parent *tp);
void _ompc_set_thread(struct ompc_thread *tp);

void _ompc_atomic_init_lock ();
void _ompc_atomic_lock ();
void _ompc_atomic_unlock ();
void _ompc_atomic_destroy_lock ();

void _ompc_critical_init ();
void _ompc_critical_destroy ();
void _ompc_enter_critical (_ompc_lock_t **);
void _ompc_exit_critical (_ompc_lock_t **);

void _ompc_set_runtime_schedule(char *s);

#ifndef __GNUC__
# define asm(X) __asm(X)
#else
# define asm(X) __asm volatile (X)
#endif

#ifdef OMNI_CPU_I386
# define MBAR() { /* asm("cpuid"); */ }
#else
# ifdef OMNI_CPU_SPARC
#  define MBAR() asm("stbar"); 
# else
#  define MBAR() 
# endif
#endif

extern int _ompc_debug_flag;

#ifdef USE_LOG
extern int _ompc_log_flag;
void tlog_init(char *name);
void tlog_finalize(void);
void tlog_parallel_IN(int id);
void tlog_parallel_OUT(int id);
void tlog_barrier_IN(int id);
void tlog_barrier_OUT(int id);
void tlog_loop_init_EVENT(int id);
void tlog_loop_next_EVENT(int id);
void tlog_section_EVENT(int id);
void tlog_single_EVENT(int id);
void tlog_critial_IN(int id);
void tlog_critial_OUT(int id);
#endif

#endif /* _OMPC_THREAD_H */
