#include "utl/Loader.h"
#include "obj/Data.h"
#include "os/Debug.h"
#include "os/Platform.h"
#include "utl/FilePath.h"
#include "utl/MemMgr.h"

LoadMgr TheLoadMgr;

void LoadMgr::SetEditMode(bool flag) {
    mEditMode = flag;
    static DataNode &edit_mode = DataVariable("edit_mode");
    edit_mode = mEditMode;
}

DataNode OnSetEditMode(DataArray *a) {
    TheLoadMgr.SetEditMode(a->Int(1));
    return 0;
}

Loader::Loader(const FilePath &fp, LoaderPos pos)
    : mPos(pos), mFile(fp), unk14(-1), mHeap(GetCurrentHeapNum()) {
    MILO_ASSERT(MemNumHeaps() == 0 || (mHeap != kNoHeap && mHeap != kSystemHeap), 0x1F0);
}

LoadMgr::~LoadMgr() {}

LoadMgr::LoadMgr()
    : mPlatform(kPlatformXBox), mEditMode(false), mCacheMode(false), mPeriod(10.0f),
      mAsyncUnload(0), mLoaderPos(kLoadFront) {}
