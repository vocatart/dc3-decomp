#include "rndobj/MotionBlur.h"
#include "rndobj/Draw.h"
#include "rndobj/PostProc.h"
#include "utl/BinStream.h"

void RndMotionBlur::Save(BinStream &bs) {
    bs << 1;
    SAVE_SUPERCLASS(Hmx::Object)
    SAVE_SUPERCLASS(RndDrawable)
    bs << mDrawList;
}

void RndMotionBlur::DrawShowing() {
    RndPostProc *cur = RndPostProc::Current();
    if (cur) {
        for (ObjPtrList<RndDrawable>::iterator it = mDrawList.begin();
             it != mDrawList.end();
             ++it) {
            cur->QueueMotionBlurObject(*it);
        }
    }
}

BEGIN_PROPSYNCS(RndMotionBlur)
    SYNC_PROP(draw_list, mDrawList)
    SYNC_SUPERCLASS(RndDrawable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

BEGIN_COPYS(RndMotionBlur)
    COPY_SUPERCLASS(Hmx::Object)
    COPY_SUPERCLASS(RndDrawable)
    CREATE_COPY(RndMotionBlur)
    BEGIN_COPYING_MEMBERS
        COPY_MEMBER(mDrawList)
    END_COPYING_MEMBERS
END_COPYS

BEGIN_LOADS(RndMotionBlur)
    LOAD_REVS(bs);
    ASSERT_REVS(1, 0);
    LOAD_SUPERCLASS(Hmx::Object)
    LOAD_SUPERCLASS(RndDrawable)
    bs >> mDrawList;
END_LOADS

DataNode RndMotionBlur::OnAllowedDrawable(const DataArray *da) {
    int allowcount = 0;
    for (ObjDirItr<RndDrawable> it(Dir(), true); it != 0; ++it) {
        if (CanMotionBlur(it))
            allowcount++;
    }
    DataArrayPtr ptr(new DataArray(allowcount));
    allowcount = 0;
    for (ObjDirItr<RndDrawable> it(Dir(), true); it != 0; ++it) {
        if (CanMotionBlur(it)) {
            ptr->Node(allowcount++) = &*it;
        }
    }
    return ptr;
}

BEGIN_HANDLERS(RndMotionBlur)
    HANDLE(allowed_drawable, OnAllowedDrawable)
    HANDLE_SUPERCLASS(RndDrawable)
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

RndMotionBlur::RndMotionBlur() : mDrawList(this) {}
