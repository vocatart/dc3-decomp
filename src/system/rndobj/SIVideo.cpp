#include "rndobj/SIVideo.h"
#include "os/Debug.h"
#include "utl/MemMgr.h"

void SIVideo::Reset() {
    mMagic = mWidth = mHeight = mBpp = 0;
    if (mData) {
        MemFree(mData, __FILE__, 22);
        mData = nullptr;
    }
}

inline int SIVideo::Bpp() const { return mBpp == 8 ? 4 : 8; }
inline int SIVideo::FrameSize() const { return (mMagic * mWidth * Bpp()) >> 3; }

char *SIVideo::Frame(int i) {
    if (mData) {
        return &mData[FrameSize() * i];
    } else
        return nullptr;
}

void SIVideo::Load(BinStream &bs, bool load_data) {
    int magic, dump, unused;
    bs >> magic;
    if (magic != 0x5349565FU) { // "SIV_"
        mMagic = magic;
        bs >> mWidth;
        bs >> mHeight;
        bs >> dump;
        bs >> unused;
        bs >> unused;
        bs >> unused;
        bs >> unused;
        mBpp = 8;
    } else {
        uint x;
        bs >> x;
        if (x > 1)
            MILO_FAIL("Can't load new SIVideo.\n");
        bs >> mMagic;
        bs >> mWidth;
        bs >> mHeight;
        bs >> mBpp;
    }
    if (mData) {
        MemFree(mData, __FILE__, 0x41);
        mData = nullptr;
    }
    if (!load_data) {
        mData = (char *)MemAlloc(mHeight * FrameSize(), __FILE__, 70, "SIVideo_buf");
        bs.Read(mData, mHeight * FrameSize());
    }
}
