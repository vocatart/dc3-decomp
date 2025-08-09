#pragma once
#include "utl/AllocInfo.h"
#include "utl/KeylessHash.h"

// size 0x1820c
class MemTracker {
public:
    MemTracker(int, int);

    void *mHashMem; // 0x0
    KeylessHash<void *, AllocInfo *> mHashTable; // 0x4
};
