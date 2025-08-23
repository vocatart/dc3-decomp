#include "rndobj/Line.h"
#include "obj/Object.h"
#include "math/Rot.h"

BEGIN_CUSTOM_PROPSYNC(RndLine::Point)
    SYNC_PROP(point, o.point)
    SYNC_PROP(color, o.color)
    SYNC_PROP(alpha, o.color.alpha)
END_CUSTOM_PROPSYNC

RndLine::RndLine()
    : mWidth(1), mHasCaps(true), mLinePairs(false), mFoldAngle(PI / 2), mMat(this),
      unk134(true) {
    mMesh = Hmx::Object::New<RndMesh>();
    mMesh->SetTransParent(this, false);
    UpdateInternal();
}
