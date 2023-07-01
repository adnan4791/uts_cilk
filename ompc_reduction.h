/* 
 * $Id: ompc_reduction.h,v 1.1.1.1 2004-02-06 18:14:49 msato Exp $
 * $RWC_Release$
 * $RWC_Copyright$
 */
/*
 *  for reduction operation in Omni runtime
 */

typedef struct { float re,im; } ompc_complex_float;	/* float complex */
typedef struct { double re,im; } ompc_complex_double;	/* double complex */

typedef union {
    char c;
    unsigned char uc;
    short s;
    unsigned short us;
    int i;
    unsigned int ui;
    long l;
    unsigned long ul;
    long long ll;
    unsigned long long ull;
    float f;
    double d;
#ifdef HAS_LONGDOUBLE
    long double ld;
#endif /* HAS_LONGDOUBLE */
    ompc_complex_float cf;
    ompc_complex_double cd;
} any_type;

#define OMPC_REDUCTION_CHAR 2
#define OMPC_REDUCTION_UNSIGNED_CHAR 3
#define OMPC_REDUCTION_SHORT 4
#define OMPC_REDUCTION_UNSIGNED_SHORT 5
#define OMPC_REDUCTION_SIGNED 6
#define OMPC_REDUCTION_INT 7
#define OMPC_REDUCTION_UNSIGNED_INT 8
#define OMPC_REDUCTION_LONG 9
#define OMPC_REDUCTION_UNSIGNED_LONG 10
#define OMPC_REDUCTION_LONGLONG 11
#define OMPC_REDUCTION_UNSIGNED_LONGLONG 12
#define OMPC_REDUCTION_FLOAT 13
#define OMPC_REDUCTION_DOUBLE  14
#define OMPC_REDUCTION_LONG_DOUBLE 15
#define OMPC_REDUCTION_MAX_TYPES 16

#define OMPC_REDUCTION_PLUS 6
#define OMPC_REDUCTION_MINUS 7
#define OMPC_REDUCTION_MUL 8
#define OMPC_REDUCTION_BITAND 9
#define OMPC_REDUCTION_BITOR 10
#define OMPC_REDUCTION_BITXOR  11
#define OMPC_REDUCTION_LOGAND 12
#define OMPC_REDUCTION_LOGOR 13
#define OMPC_REDUCTION_MIN 14
#define OMPC_REDUCTION_MAX 15
#define OMPC_REDUCTION_MAX_OPS 16

/* extended for Fortran Complex */
#define OMPC_REDUCTION_COMPLEX_FLOAT 16
#define OMPC_REDUCTION_COMPLEX_DOUBLE 17


