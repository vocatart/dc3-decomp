#pragma once
#include "utl/MemMgr.h"

/** Used for decrypting encrypted BinStreams */
class Rand2 {
private:
    int mSeed;

public:
    Rand2(int);
    int Int();

    NEW_OVERLOAD("Rand2", 18);
    DELETE_OVERLOAD("Rand2", 18);
};
