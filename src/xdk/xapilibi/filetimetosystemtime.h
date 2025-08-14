#pragma once
#include "../win_types.h"
#include "gettime.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL FileTimeToSystemTime(const FILETIME *, SYSTEMTIME *);

#ifdef __cplusplus
}
#endif
