#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

HANDLE CreateEventA(void *, BOOL, BOOL, LPCSTR);

#ifdef __cplusplus
}
#endif
