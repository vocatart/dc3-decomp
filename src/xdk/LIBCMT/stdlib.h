#pragma once
#include "size_def.h"

#ifdef __cplusplus
extern "C" {
#endif

void qsort(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *));

#ifdef __cplusplus
}
#endif
