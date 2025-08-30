#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

HRESULT NuiWaveGetGestureOwnerProgress(DWORD *, float *);
HRESULT NuiWaveSetEnabled(BOOL);

#ifdef __cplusplus
}
#endif
