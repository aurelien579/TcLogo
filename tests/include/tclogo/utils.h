#ifndef UTILS_H
#define UTILS_H

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define alloc(T) ((T *) malloc(sizeof(T)))
#define alloc_n(T, n) ((T *) malloc(sizeof(T) * n))

#endif
