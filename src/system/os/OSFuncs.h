#pragma once
#include "xdk/xapilibi/getcurrentthreadid.h"

bool MainThread();

extern unsigned int gMainThreadID;

inline bool MainThread() {
    if (gMainThreadID == -1)
        return true;
    if (GetCurrentThreadId() == gMainThreadID)
        return true;
    return false;
}
