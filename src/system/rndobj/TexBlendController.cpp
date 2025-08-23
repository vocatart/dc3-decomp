#include "rndobj/TexBlendController.h"

RndTexBlendController::RndTexBlendController()
    : mMesh(this), mObject1(this), mObject2(this), mReferenceDistance(0), mMinDistance(0),
      mMaxDistance(0), mTex(this) {}
