#pragma once
#include "xuser.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _XUSER_SIGNIN_INFO { /* Size=0x28 */
    _XUSER_SIGNIN_INFO()
        : dwInfoFlags(0), UserSigninState(eXUserSigninState_NotSignedIn),
          dwGuestNumber(0), dwSponsorUserIndex(0) {}
    XUID xuid; // 0x0
    DWORD dwInfoFlags; // 0x8
    XUSER_SIGNIN_STATE UserSigninState; // 0xc
    DWORD dwGuestNumber; // 0x10
    DWORD dwSponsorUserIndex; // 0x14
    char szUserName[16]; // 0x18
};
typedef _XUSER_SIGNIN_INFO XUSER_SIGNIN_INFO;

DWORD XUserGetSigninInfo(DWORD, DWORD, XUSER_SIGNIN_INFO *);

#ifdef __cplusplus
}
#endif
