#include "obj/Dir.h"
#include "Object.h"
#include "os/Debug.h"
#include "utl/MemMgr.h"
#include "utl/Symbol.h"
#include "utl/BinStream.h"

BinStream &operator>>(BinStream &bs, InlineDirType &ty) {
    unsigned char uc;
    bs >> uc;
    ty = (InlineDirType)uc;
    return bs;
}

void ObjectDir::SetInlineProxyType(InlineDirType t) {
    MILO_ASSERT(t != kInlineCachedShared, 0x198);
    mInlineProxyType = t;
}

void ObjectDir::Reserve(int hashSize, int stringSize) {
    MemTemp tmp;
    if (mHashTable.Size() < hashSize) {
        mHashTable.Resize(hashSize, 0);
    }
    mStringTable.Reserve(stringSize);
}

ObjectDir::InlinedDir::InlinedDir() : dir(), file() {}
ObjectDir::InlinedDir::~InlinedDir() {}

ObjectDir::ObjectDir()
    : mHashTable(0, Entry(), Entry(), 0), mStringTable(0), mProxyOverride(false),
      mInlineProxyType(kInlineCached), mLoader(nullptr), mIsSubDir(false),
      mInlineSubDirType(kInlineNever), mPathName(gNullStr), mViewports(7),
      mCurViewportID((ViewportId)0), unk8c(nullptr), mCurCam(nullptr), mAlwaysInlined(0),
      mAlwaysInlineHash(gNullStr) {
    ResetViewports();
}

ObjectDir::~ObjectDir() { mSubDirs.clear(); }
