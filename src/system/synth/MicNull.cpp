#include "synth/MicNull.h"
#include "math/Rand.h"

Rand sRand(0x1bca7);

short *MicNull::GetRecentBuf(int &size) {
    size = DIM(mRecentBuf);
    memcpy(mRecentBuf, mBuf, sizeof(mRecentBuf));
    return mRecentBuf;
}

MicNull::MicNull() {
    for (int i = 0; i < 10000U; i++) {
        mBuf[i] = sRand.Int(-32000, 32000);
    }
}
