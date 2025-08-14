#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _SYSTEMTIME { /* Size=0x10 */
    /* 0x0000 */ unsigned short wYear;
    /* 0x0002 */ unsigned short wMonth;
    /* 0x0004 */ unsigned short wDayOfWeek;
    /* 0x0006 */ unsigned short wDay;
    /* 0x0008 */ unsigned short wHour;
    /* 0x000a */ unsigned short wMinute;
    /* 0x000c */ unsigned short wSecond;
    /* 0x000e */ unsigned short wMilliseconds;
};
typedef _SYSTEMTIME SYSTEMTIME;

struct _FILETIME { /* Size=0x8 */
    /* 0x0000 */ DWORD dwHighDateTime;
    /* 0x0004 */ DWORD dwLowDateTime;
};
typedef _FILETIME FILETIME;

void GetLocalTime(SYSTEMTIME *);
void GetSystemTime(SYSTEMTIME *);

#ifdef __cplusplus
}
#endif
