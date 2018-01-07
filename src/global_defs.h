#ifndef __GLOBAL_DEFS__
#define __GLOBAL_DEFS__
#define SQR(X) ((X)*(X))
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define BUFSIZE 500 /* standard size for a buffer */
#define NOT_IN -1
#define IN 0
#define LEFT 1
#define RIGHT 2
#define THRESHOLD 0.01
#define MAX_ITERATIONS 10
#define ROW_MAJOR 0
#define COL_MAJOR 1
typedef enum { false, true } bool;
#endif
