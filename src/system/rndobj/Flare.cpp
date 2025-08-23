#include "rndobj/Flare.h"

RndFlare::RndFlare()
    : mPointTest(true), mAreaTest(true), mVisible(false), mSizes(0.1, 0.1), mMat(this),
      mRange(0, 0), mOffset(0), mSteps(1), mStep(0), unk144(0), unk148(false),
      unk149(false), unk17c(1, 1) {
    mMatrix.Identity();
}
