#pragma once
#include "size_def.h"

#define NULL 0

#ifdef __cplusplus
extern "C" {
#endif

typedef int ptrdiff_t;
#define offsetof(T, mem) ((int)&((T *)0->mem))

#ifdef __cplusplus
}
#endif
