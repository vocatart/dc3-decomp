#include "os/PlatformMgr.h"
#include "obj/Data.h"
#include "obj/DataUtl.h"
#include "os/Debug.h"
#include "os/System.h"
#include "os/User.h"
#include "os/UserMgr.h"
#include "utl/Option.h"
#include "utl/UTF8.h"
#include "utl/Str.h"
#include "xdk/XAPILIB.h"

PlatformMgr ThePlatformMgr;

bool PlatformMgr::IsAnyUserSignedIntoLive() const {
    for (int i = 0; i < 4; i++) {
        if (IsSignedIntoLive(i))
            return true;
    }
    return false;
}

PlatformRegion PlatformMgr::GetRegion() const { return mRegion; }

Symbol PlatformRegionToSymbol(PlatformRegion r) {
    MILO_ASSERT(r < kNumRegions, 0x29);
    static Symbol sym[4] = { "", "na", "europe", "japan" };
    return sym[r];
}

PlatformRegion SymbolToPlatformRegion(Symbol s) {
    for (int i = 0; i < 4; i++) {
        PlatformRegion r = (PlatformRegion)i;
        if (PlatformRegionToSymbol(r) == s)
            return r;
    }
    MILO_FAIL("Invalid region %s", s);
    return kNumRegions;
}

void UTF8FilterKeyboardString(char *c, int i, const char *cc) {
    static Symbol platform_mgr("platform_mgr");
    static Symbol keyboard_allowed_chars("keyboard_allowed_chars");
    static const char *allowed =
        SystemConfig(platform_mgr)->FindStr(keyboard_allowed_chars);
    UTF8FilterString(c, i, cc, allowed, '?');
}

bool PlatformMgr::IsSignedIn(int padnum) const {
    if (padnum < 0) {
        MILO_FAIL("PadNum = %d", padnum);
    }
    return 1 << padnum & mSigninMask;
}

bool PlatformMgr::IsUserSignedIn(const LocalUser *pUser) const {
    MILO_ASSERT(pUser, 0x51);
    return IsSignedIn(pUser->GetPadNum());
}

bool PlatformMgr::IsPadNumSignedIn(int padnum) const { return IsSignedIn(padnum); }

bool PlatformMgr::HasPadNumsSigninChanged(int padnum) const {
    if (padnum < 0) {
        MILO_FAIL("PadNum = %d", padnum);
    }
    return 1 << padnum & mSigninChangeMask;
}

bool PlatformMgr::HasUserSigninChanged(const LocalUser *pUser) const {
    MILO_ASSERT(pUser, 99);
    return HasPadNumsSigninChanged(pUser->GetPadNum());
}

bool PlatformMgr::IsUserSignedIntoLive(const LocalUser *pUser) const {
    MILO_ASSERT(pUser, 0x6A);
    return IsSignedIntoLive(pUser->GetPadNum());
}

bool PlatformMgr::UserHasOnlinePrivilege(const LocalUser *pUser) const {
    MILO_ASSERT(pUser, 0x7A);
    return HasOnlinePrivilege(pUser->GetPadNum());
}

bool PlatformMgr::IsUserAGuest(const LocalUser *pUser) const {
    MILO_ASSERT(pUser, 0x87);
    int padnum = pUser->GetPadNum();
    if (padnum < 0) {
        MILO_FAIL("PadNum = %d", padnum);
    }
    return IsPadAGuest(padnum);
}

void PlatformMgr::ShowUserFriendsUI(const LocalUser *pUser) {
    MILO_ASSERT(pUser, 0xA6);
    ShowFriendsUI(pUser->GetPadNum());
}

void PlatformMgr::ShowOfferUI(const LocalUser *user) {
    MILO_ASSERT(user, 0xAC);
    ShowOfferUI(user->GetPadNum());
}

bool PlatformMgr::ShowUserPartyUI(const LocalUser *pUser) {
    MILO_ASSERT(pUser, 0xB2);
    return ShowPartyUI(pUser->GetPadNum());
}

void PlatformMgr::InviteUserParty(const LocalUser *pUser) {
    MILO_ASSERT(pUser, 0xB8);
    InviteParty(pUser->GetPadNum());
}

LocalUser *PlatformMgr::GetOwnerUserOfGuestUser(LocalUser *pUser) {
    MILO_ASSERT(pUser, 0xBE);
    return TheUserMgr->GetLocalUserFromPadNum(GetOwnerOfGuest(pUser->GetPadNum()));
}

void PlatformMgr::SetRegion(PlatformRegion region) {
    const char *regionStr = OptionStr("region", nullptr);
    if (regionStr) {
        mRegion = SymbolToPlatformRegion(regionStr);
    } else {
        mRegion = region;
    }
    MILO_ASSERT(mRegion != kRegionNone, 0xCC);
    String str(PlatformRegionToSymbol(mRegion));
    str.ToUpper();
    str = MakeString("REGION_%s", str);
    DataArrayPtr ptr(1);
    DataSetMacro(str.c_str(), ptr);
}

void PlatformMgr::SetDiskError(DiskError derr) {
    if (mDiskError != kFailedChecksum && mDiskError != derr) {
        mDiskError = derr;
        if (mDiskError != kNoDiskError) {
            static DiskErrorMsg msg;
            Handle(msg, false);
        }
        DiscErrorCallbackFunc *func = GetDiskErrorCallback();
        if (func)
            func();
        while (true) {
            MILO_LOG("DISK ERROR\n");
            Sleep(1);
        }
    }
}

void PlatformMgr::DebugFakeSigninChangeMsg(int padnum) {
    if (padnum < 0) {
        MILO_FAIL("PadNum = %d", padnum);
    }
    mSigninChangeMask = 1 << padnum;
    static SigninChangedMsg msg(0, 0);
    msg[0] = mSigninMask;
    msg[1] = mSigninChangeMask;
    Handle(msg, false);
}
