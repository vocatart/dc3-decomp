#include "rndobj/PostProcMgr.h"
#include "obj/Object.h"
#include "rndobj/Poll.h"
#include "rndobj/PostProc.h"

RndPostProcMgr::RndPostProcMgr()
    : mSelectedPostProc(this), unk1c(nullptr), unk20(this), unk34(-1), unk38(-1) {
    unk1c = Hmx::Object::New<RndPostProc>();
}

BEGIN_PROPSYNCS(RndPostProcMgr)
    SYNC_PROP_SET(
        selected_postproc,
        mSelectedPostProc.Ptr(),
        mSelectedPostProc = _val.Obj<RndPostProc>()
    )
    SYNC_SUPERCLASS(RndPollable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
