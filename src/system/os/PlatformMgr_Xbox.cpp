#include "os/PlatformMgr.h"
#include "xdk/XAPILIB.h"

PlatformMgr::PlatformMgr() {}

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
