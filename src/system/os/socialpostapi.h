#pragma once
#include "xdk/win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _XOVERLAPPED { /* Size=0x1c */
    unsigned int InternalLow;
    unsigned int InternalHigh;
    unsigned int InternalContext;
    void *hEvent;
    void (*pCompletionRoutine)(unsigned int, unsigned int, _XOVERLAPPED *);
    unsigned int dwCompletionContext;
    unsigned int dwExtendedError;
};
typedef _XOVERLAPPED XOVERLAPPED;

DWORD XSocialGetCapabilities(DWORD *, XOVERLAPPED *);

#ifdef __cplusplus
}
#endif
