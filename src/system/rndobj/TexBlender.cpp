#include "rndobj/TexBlender.h"

RndTexBlender::RndTexBlender()
    : mBaseMap(this), mNearMap(this), mFarMap(this), mOutputTextures(this),
      mControllerList(this), mOwner(this), mControllerInfluence(1), unkbc(0),
      unkc0(true) {}
