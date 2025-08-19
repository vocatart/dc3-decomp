#include "rndobj/ColorXfm.h"
#include "utl/BinStream.h"

void RndColorXfm::Reset() { mColorXfm.Reset(); }

void RndColorXfm::AdjustBrightness() {
    Transform tf;
    tf.Reset();
    float set = (mBrightness + 100.0f) / 200.0f + -0.5f;
    tf.v.Set(set, set, set);
    Multiply(mColorXfm, tf, mColorXfm);
}

void RndColorXfm::Save(BinStream &bs) const {
    bs << 0;
    bs << mColorXfm;
    bs << mHue << mSaturation << mLightness;
    bs << mContrast << mBrightness;
    bs << mLevelInLo << mLevelInHi;
    bs << mLevelOutLo << mLevelOutHi;
}

bool RndColorXfm::Load(BinStream &bs) {
    int rev;
    bs >> rev;
    if (rev > 0)
        return false;
    else {
        bs >> mColorXfm;
        bs >> mHue >> mSaturation >> mLightness >> mContrast >> mBrightness;
        bs >> mLevelInLo >> mLevelInHi;
        bs >> mLevelOutLo >> mLevelOutHi;
        return true;
    }
}

RndColorXfm::RndColorXfm()
    : mHue(0), mSaturation(0), mLightness(0), mContrast(0), mBrightness(0),
      mLevelInLo(0, 0, 0), mLevelInHi(1, 1, 1), mLevelOutLo(0, 0, 0),
      mLevelOutHi(1, 1, 1) {
    Reset();
}

void RndColorXfm::AdjustColorXfm() {
    mColorXfm.Reset();
    AdjustHue();
    AdjustSaturation();
    AdjustLightness();
    AdjustContrast();
    AdjustBrightness();
    AdjustLevels();
}
