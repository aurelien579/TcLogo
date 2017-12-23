#ifndef _PANIC_H_
#define _PANIC_H_

#include <stdio.h>
#include <stdlib.h>

#define panic(msg) do {                                                 \
    fprintf(stderr, "[PANIC] %s at %s:%d\n", msg, __FILE__, __LINE__);  \
    exit(-1);                                                           \
} while(0);                                                             \

#endif
