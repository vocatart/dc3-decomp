#include "obj/Dir.h"
#include "Object.h"
#include "os/Debug.h"
#include "utl/Loader.h"
#include "utl/MemMgr.h"
#include "utl/Symbol.h"
#include "utl/BinStream.h"

const char *kNotObjectMsg = "Could not find %s in dir \"%s\"";

BinStream &operator>>(BinStream &bs, InlineDirType &ty) {
    unsigned char uc;
    bs >> uc;
    ty = (InlineDirType)uc;
    return bs;
}

bool ObjectDir::SaveSubdirs() {
    return !IsProxy() || mProxyFile.empty() || gLoadingProxyFromDisk;
}

bool ObjectDir::ShouldSaveProxy(BinStream &bs) {
    return IsProxy() && (!mProxyFile.empty() || InlineProxy(bs));
}

void ObjectDir::SetInlineProxyType(InlineDirType t) {
    MILO_ASSERT(t != kInlineCachedShared, 0x198);
    mInlineProxyType = t;
}

void ObjectDir::Load(BinStream &bs) {
    PreLoad(bs);
    PostLoad(bs);
    if (IsProxy() && !mProxyFile.empty()) {
        // class Loader *ldr = TheLoadMgr.GetLoader(mProxyFile);
        // if (ldr)
        //     TheLoadMgr.PollUntilLoaded(ldr, 0);
    }
}

void ObjectDir::OldLoadProxies(BinStream &bs, int i) {
    int x;
    bs >> x;
    if (x != 0)
        MILO_FAIL("Proxies not allowed here");
}

BinStreamRev &operator>>(BinStreamRev &bs, ObjectDir::Viewport &v) {
    // bs >> transform
    return bs;
}

void ObjectDir::TransferLoaderState(ObjectDir *dir) {
    mProxyFile = dir->mProxyFile;
    mProxyOverride = dir->mProxyOverride;
    mLoader = dir->mLoader;
    dir->mLoader = nullptr;
}

bool ObjectDir::HasDirPtrs() const {
    if (sDeleting == this) {
        return true;
    } else {
        for (ObjRef::iterator it = mRefs.begin(); it != mRefs.end(); ++it) {
            if (it->IsDirPtr())
                return true;
        }
        return false;
    }
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
