#pragma once
#include "size_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t ptrdiff_t;
#define offsetof(T, mem) ((int)&((T *)0->mem))

#ifdef __cplusplus
}
#endif
