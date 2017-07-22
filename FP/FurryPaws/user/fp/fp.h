/* fp.h */


#ifndef FP_H
#define FP_H


#define GENERATED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <string.h>
#include <time.h>

#ifndef __MINGW32__
# define PRECISE_CLOCK
# include <sys/time.h>
# include <sys/resource.h>
#endif


#if defined(STRESS_GC) || defined(EXTRA_CAREFUL)
# ifndef CAREFUL
#  define CAREFUL
# endif
#endif

#define ERROR_FILE          "stderr.txt"

#ifndef DEFAULT_HEAP_SIZE
# define DEFAULT_HEAP_SIZE  2000000
#endif

// percentage when debug-mod will trigger warnings
#define HEAP_CRITICAL_SIZE  25

#define DEFAULT_ATOM_TABLE_SIZE 1000

#define WORD_SIZE           (sizeof(void *) * 8)

#define NUMBER_BIT          1
#define ATOM_BIT            2
#define NONSEQ_MASK         (NUMBER_BIT | ATOM_BIT)
#define GCMARK_BIT          (1UL << (WORD_SIZE - 1))

#define NUMBER_TYPE         NUMBER_BIT
#define ATOM_TYPE           ATOM_BIT
#define SEQUENCE_TYPE       0

#define SAVED_AREA_SIZE     10000
#define ROOT_SLOTS          16
#define DUMP_ELEMENT_LIMIT  100
#define DEFAULT_CALL_TRACE_LENGTH 16
#define DEFAULT_HANDY_BUFFER_SIZE 10000
#define MESSAGE_BUFFER_SIZE 1024
#define MAX_HASH_COUNT      64

#define LF_ROOT_SLOT        0

#ifdef CAREFUL
# define CHECK_FPTR(x)      check_fptr(x)
#else
# define CHECK_FPTR(x)
#endif

#ifdef CAREFUL
# define INLINE             static
#else
# define INLINE             static inline
#endif

#ifdef STACK_LIMIT
// assumes stack grows downward
# define CHECK_STACK  \
  char unused;					\
  if(&unused < stack_base - STACK_LIMIT)	\
    bomb("stack limit reached - aborting")
#else
# define CHECK_STACK
#endif

#define DEFINE(name)        static X name(X x)
#define XDEFINE(name)       X name(X x)
#define OP2DEFINE(name)     static X name ## _5fop2(X x, X y)

#ifdef CAREFUL
# define ENTRY              X *sp0 = savedp; CHECK_STACK; CONSULTCACHE(x)
# define RETURN(x)          {UPDATECACHE(x); check_saved(sp0); return check_fptr(x);}
#else
# define ENTRY              CONSULTCACHE(x)
# define RETURN(x)          {UPDATECACHE(x); return(x);}
#endif

#ifdef RCACHE
# define CONSULTCACHE(x)    static int g; static X r, a, a0; a0 = x; \
                            if(g == gc_count && x == a) { ++rcache_hits; return r; }
# define UPDATECACHE(x)     g = gc_count; a = a0; r = x
#else
# define CONSULTCACHE(x)
# define UPDATECACHE(x)
#endif


typedef void *X;

typedef struct S {
  unsigned long length;
  X data[ 1 ];
} S;

typedef char *A;

struct bucket {
  char *str;
  struct bucket *next;
};


#define IS_N(x)             ((long)(x) & NUMBER_BIT)
#define IS_A(x)             (((long)(x) & NONSEQ_MASK) == ATOM_BIT)
#define IS_S(x)             (((long)(x) & NONSEQ_MASK) == 0)
#define N_VALUE(x)          ((long)(x) >> 1)
#define TO_N(n)             ((X)(long)(((n) << 1) | NUMBER_BIT))
#define A_STRING(x)         ((char *)((unsigned long)(x) & ~ATOM_BIT))
#define TO_A(ptr)           ((X)((unsigned long)(ptr) | ATOM_BIT))
#define S_LENGTH(x)         (((S *)(x))->length)
#define S_DATA(x)           (((S *)(x))->data)
#define TO_B(x)             ((x) ? T : F)


#define error_output        ensure_error_output()

static int debug_flag = 0;
static int no_strings_flag = 0;
static X *fromspace_start, *fromspace_end, *tospace_start, *tospace_end;
static X *fromspace_top, *tospace_top;
static int gc_count = 0;
static int rcache_hits = 0;
static X saved[ SAVED_AREA_SIZE ];
static X *savedp;
static char *buffer;
static int buffer_size;
static int lf_created = 0;
static int log_errors = 0;
static char message_buffer[ MESSAGE_BUFFER_SIZE ];

static struct bucket **atom_table;
static int atom_table_size;
static int atom_table_count;
static X T, F, ERROR, UNDEFINED, EMPTY, FPTR, IO;

static char **calltrace, **calltraceend, **calltracetop;
static jmp_buf *catcher;
static char *stack_base;
static char *max_stack_top;

static void dribble(char *msg, ...);
static X bomb(char *msg, ...);	/* doesn't throw */
static X fail(char *msg, ...);	/* throws */
static X failx(X x, char *msg, ...);	/* throws */
static X uncaught(X x);
static int init(int argc, char *argv[], X (*main)(X), void (*conslf)());
static X string(char *str);
static void dump(X x, FILE *fp, int limited);
static int is_string(X x, int nl);
static FILE *ensure_error_output();

#ifndef NDEBUG
static void dumptrace(FILE *fp);
static void tracecall(char *loc);
#else
# define tracecall(loc)
# define dumptrace(fp)
#endif

#ifdef CAREFUL
static void check_saved(X *sp0);
#endif

static void init_heap(unsigned int size);
static unsigned int shrink_heap(unsigned int size);
static unsigned int reclaim();
static X allocate(unsigned long length);
static X sequence(unsigned long length, ...);
static X fpointer(void *fptr);

static void init_atoms(unsigned int size);
static X intern(char *str);

#ifdef UNSAFE
# define check_N(x, loc)   N_VALUE(x)
# define check_A(x, loc)   A_STRING(x)
# define check_S(x, loc)   S_DATA(x)
# define check_Smin(x, minlen, loc)  S_DATA(x)
#else
static long check_N(X x, char *loc);
static char *check_A(X x, char *loc);
static X *check_S(X x, char *loc);
static X *check_Sn(X x, unsigned long len, char *loc);
static X *check_Smin(X x, unsigned long minlen, char *loc);
#endif

static char *check_string(X x, char *loc, int nl);
static X check_fptr(X x);

static int eq1(X x, X y);
static X select1(X x, X y);

DEFINE(___add);
DEFINE(___sub);
DEFINE(___mul);
DEFINE(___div);
DEFINE(___mod);
DEFINE(___band);
DEFINE(___bor);
DEFINE(___bxor);
OP2DEFINE(___add);
OP2DEFINE(___sub);
OP2DEFINE(___mul);
OP2DEFINE(___div);
OP2DEFINE(___mod);
OP2DEFINE(___band);
OP2DEFINE(___bor);
OP2DEFINE(___bxor);
DEFINE(___id);
DEFINE(___eq);
DEFINE(____5f);
DEFINE(___s1);
DEFINE(___s2);
DEFINE(___s3);
DEFINE(___s4);
DEFINE(____25trace);
DEFINE(___al);
DEFINE(___ar);
DEFINE(___tl);
DEFINE(___ton);
DEFINE(___toa);
DEFINE(___tos);
DEFINE(___cat);
DEFINE(___atom);
DEFINE(___len);
DEFINE(___num);
DEFINE(___select);
DEFINE(____5fin);
DEFINE(____5fout);
DEFINE(____5femit);
DEFINE(____5fget);
DEFINE(____5fshow);
DEFINE(____5fsystem);
DEFINE(____5fenv);
DEFINE(____5fthrow);
DEFINE(____5frnd);
DEFINE(___rev);
DEFINE(___cmp);
OP2DEFINE(___cmp);
DEFINE(___string);
DEFINE(____5fgc);
DEFINE(___app);
DEFINE(___subseq);
DEFINE(____5fiostep);
DEFINE(___make);
DEFINE(___iota);
DEFINE(___dl);
DEFINE(___dr);


#if defined(GENERATED) && !defined(STANDALONE)
# include <fp/gc.c>
# include <fp/ht.c>
# include <fp/rt.c>
# include <fp/lib.c>
#endif


#endif
