#include "rndobj/HiResScreen.h"
#include "utl/MemMgr.h"

HiResScreen gHiResScreen;
HiResScreen &TheHiResScreen = gHiResScreen;

void HiResScreen::BmpCache::DeleteCache() {
    for (unsigned int i = 0; i < mTotalNumCacheLines; i++) {
        FileDelete(mFileNames[i].c_str());
    }
}

int HiResScreen::GetPaddingX() const { return 480; }
int HiResScreen::GetPaddingY() const { return 270; }

HiResScreen::BmpCache::BmpCache(unsigned int ui1, unsigned int ui2) {
    mRowsPerCacheLine = ui2 + 1;
    mPixelsPerRow = ui1;
    mTotalRows = ui2;
    mDirtyStart = 0;
    mDirtyEnd = 0;
    mByteSize = mTotalRows % mRowsPerCacheLine;
    MILO_ASSERT(mTotalRows % mRowsPerCacheLine == 0, 0x3B);
    mTotalNumCacheLines = mTotalRows / mRowsPerCacheLine;
    mFileNames = new String[mTotalNumCacheLines];
    for (uint i = 0; i < mTotalNumCacheLines; i++) {
        mFileNames[i] = MakeString("_hires_cache_%.2d.dat", i);
    }
    mBuffer = (unsigned char *)MemAlloc(mByteSize, __FILE__, 0x44, "HiResScreenCache");
    mCurrLoadedIndex = ui2;
    DeleteCache();
}

HiResScreen::BmpCache::~BmpCache() {
    DeleteCache();
    delete[] mFileNames;
    mFileNames = 0;
    delete mBuffer;
    mBuffer = 0;
}
