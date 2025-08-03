#pragma once

#include "utl/MemMgr.h"
class CriticalSection {
public:
    // int mEntryCount;
    // OSMutex mCritSec;

    CriticalSection();
    ~CriticalSection();
    void Enter();
    void Exit();
    bool TryEnter();
    void Abandon();

    NEW_OVERLOAD("CriticalSection", 0x20);
    DELETE_OVERLOAD("CriticalSection", 0x20);
};

class CritSecTracker {
public:
    CriticalSection *mCritSec;

    CritSecTracker(CriticalSection *section) {
        mCritSec = section;
        if (section != 0) {
            section->Enter();
        }
    }

    ~CritSecTracker() {
        if (mCritSec != 0) {
            mCritSec->Exit();
        }
    }
};
