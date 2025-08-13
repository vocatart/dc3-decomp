#include "os/PlatformMgr.h"
#include "xdk/XAPILIB.h"
#include "xdk/XMP.h"
#include "xdk/XNET.h"
#include "xdk/NUI.h"

namespace {
    int mSigninSameGuest;
    XUID mXuidCache[4];
    int gNumSmartGlassClients;
}

PlatformMgr::PlatformMgr() {}

bool PlatformMgr::IsEthernetCableConnected() { return XNetGetEthernetLinkStatus() != 0; }

void PlatformMgr::UpdateSigninState() {
    XUID oldCache[4] = { mXuidCache[0], mXuidCache[1], mXuidCache[2], mXuidCache[3] };
    mSigninSameGuest = 0;
    for (int i = 0; i < 4; i++) {
        if (XUserGetSigninState(i) == eXUserSigninState_NotSignedIn) {
            mXuidCache[i] = 0;
        } else {
            XUSER_SIGNIN_INFO info;
            mSigninMask |= 1 << i;
            XUserGetSigninInfo(i, 2, &info);
            XUserGetXUID(i, &info.xuid);
            mXuidCache[i] = 0;
        }
        if (oldCache[i] != mXuidCache[i]) {
            mSigninChangeMask |= 1 << i;
            if (mXuidCache[i] == 0) {
                mSigninSameGuest |= 1 << i;
            }
        }
    }
}

bool PlatformMgr::HasCreatedContentPrivilege() const {
    bool ret = true;
    for (int i = 0; i < 4; i++) {
        int bptr = 0;
        if (XUserCheckPrivilege(i, XPRIVILEGE_USER_CREATED_CONTENT, &bptr) == 0
            && XUserCheckPrivilege(i, XPRIVILEGE_USER_CREATED_CONTENT_FRIENDS_ONLY, &bptr)
                == 0) {
            ret &= bptr;
        }
    }
    return ret;
}

bool PlatformMgr::HasKinectSharePrvilege() const {
    int bptr = 0;
    if (XUserCheckPrivilege(0xFF, XPRIVILEGE_SHARE_CONTENT_OUTSIDE_LIVE, &bptr) == 0) {
        return true;
    } else if (bptr == 0) {
        bool ret = bptr;
        return ret;
    }
}

bool PlatformMgr::IsSmartGlassConnected() { return gNumSmartGlassClients > 0; }

void PlatformMgr::SetPadContext(int i1, int i2, int i3) const {
    if (i1 != -1 && ThePlatformMgr.IsSignedIn(i1)) {
        XUserSetContext(i1, i2, i3);
    }
}

void PlatformMgr::SetPadPresence(int i1, int i2) const {
    if (i1 != -1 && ThePlatformMgr.IsSignedIn(i1)) {
        XUserSetContext(i1, 0x8001, i2);
    }
}

void PlatformMgr::ShowFriendsUI(int i1) {
    unsigned long ul;
    if (IsSignedIn(i1)) {
        if (sXShowCallback(ul)) {
            XShowNuiFriendsUI(ul, i1);
        } else {
            XShowFriendsUI(i1);
        }
    }
}

void PlatformMgr::SetBackgroundDownloadPriority(bool b1) {
    XBackgroundDownloadSetMode((XBACKGROUND_DOWNLOAD_MODE)(b1 + 1));
}

// int __cdecl ShowControllerRequiredUIThreaded(void)

bool PlatformMgr::ShowPartyUI(int i1) {
    unsigned long ul;
    unsigned long ret = 1;
    if (IsSignedIn(i1)) {
        if (sXShowCallback(ul)) {
            ret = XShowNuiPartyUI(ul, i1);
        } else {
            ret = XShowPartyUI(i1);
        }
    }
    return ret;
}

bool PlatformMgr::ShowFitnessBodyProfileUI(int i1) {
    unsigned long ul;
    unsigned long ret = 1;
    if (IsSignedIn(i1)) {
        if (sXShowCallback(ul)) {
            ret = XShowNuiFitnessBodyProfileUI(ul, i1);
        } else {
            ret = XShowFitnessBodyProfileUI(i1);
        }
    }
    return ret;
}

void PlatformMgr::EnableXMP() { XMPRestoreBackgroundMusic(); }
void PlatformMgr::DisableXMP() { XMPOverrideBackgroundMusic(); }

void PlatformMgr::SetScreenSaver(bool b1) {
    mScreenSaver = b1;
    XEnableScreenSaver(b1);
}
