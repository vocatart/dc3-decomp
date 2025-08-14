#pragma once
#include "gettime.h"
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _TIME_ZONE_INFORMATION { /* Size=0xac */
    /* 0x0000 */ int Bias;
    /* 0x0004 */ wchar_t StandardName[32];
    /* 0x0044 */ _SYSTEMTIME StandardDate;
    /* 0x0054 */ int StandardBias;
    /* 0x0058 */ wchar_t DaylightName[32];
    /* 0x0098 */ _SYSTEMTIME DaylightDate;
    /* 0x00a8 */ int DaylightBias;
};
typedef _TIME_ZONE_INFORMATION TIME_ZONE_INFORMATION;

DWORD GetTimeZoneInformation(TIME_ZONE_INFORMATION *);

#ifdef __cplusplus
}
#endif
