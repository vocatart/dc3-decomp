#include "utl/MakeString.h"
#include "os/Debug.h"
#include "os/System.h"

bool bufExceeded = false;

char *NextBuf() { return 0; }

FormatString::FormatString()
    : mBuf(NextBuf()), mBufSize(0x1000), mFmtEnd(nullptr), mType(kNone) {}

FormatString &FormatString::operator<<(int i) {
    if (mType != kInt)
        MILO_WARN(
            "FormatString: '%s' doesn't start with kInt.  Format: '%s'", mFmt, mFmtBuf
        );
    char tmp = *mFmtEnd;
    *mFmtEnd = '\0';
    int n = Hx_snprintf(mBuf + 0x1000 - mBufSize, mBufSize, mFmt, i);
    *mFmtEnd = tmp;
    if (n < 0 && !bufExceeded) {
        bufExceeded = true;
        MILO_WARN("MakeString() buffer size %d exceeded", 0x1000);
    }
    mBufSize -= n;
    UpdateType();
    return *this;
}

const char *FormatString::Str() {
    if (mType != kNone)
        MILO_WARN(
            "FormatString: '%s' doesn't start with kNone.  Format: '%s'", mFmt, mFmtBuf
        );
    if (*mFmt != '\0') {
        MILO_ASSERT(mFmtEnd - mFmt < mBufSize, 0x16F);
        char *p2 = mFmt;
        char *p3 = mBuf + 0x1000 - mBufSize;
        for (; *p2 != 0; p2++, p3++) {
            *p3 = *p2;
        }
    }
    return mBuf;
}

FormatString &FormatString::operator<<(const char *cc) {
    if (mType != kStr)
        MILO_WARN(
            "FormatString: '%s' doesn't start with kStr.  Format: '%s'", mFmt, mFmtBuf
        );
    MILO_ASSERT_FMT(
        cc < mBuf || cc >= mBuf + sizeof(mFmtBuf), "FormatString: arg in buffer"
    );
    char tmp = *mFmtEnd;
    *mFmtEnd = '\0';
    int n = Hx_snprintf(mBuf + sizeof(mFmtBuf) - mBufSize, mBufSize, mFmt, cc);
    *mFmtEnd = tmp;
    MILO_ASSERT(n >= 0, 0x12A);
    mBufSize -= n;
    UpdateType();
    return *this;
}

FormatString::FormatString(const char *str)
    : mBuf(NextBuf()), mBufSize(0x1000), mFmtEnd(nullptr), mType(kNone) {
    InitializeWithFmt(str, true);
}
