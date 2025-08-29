#include "synth/Sequence.h"
#include "math/Rand.h"

namespace {
    float RandomVal(float f1, float f2) {
        if (f2 == 0.0f)
            return f1;
        else
            return RandomFloat(f1 - f2, f1 + f2);
    }
}

Sequence::Sequence()
    : mInsts(this), mAvgVol(0.0f), mVolSpread(0.0f), mAvgTranspose(0.0f),
      mTransposeSpread(0.0f), mAvgPan(0.0f), mPanSpread(0.0f), mFaders(this),
      mCanStop(true) {}

SeqInst::SeqInst(Sequence *seq) : mOwner(seq), mVolume(0.0f), mStarted(false) {
    mRandVol = RandomVal(mOwner->AvgVol(), mOwner->VolSpread());
    mRandPan = RandomVal(mOwner->AvgPan(), mOwner->PanSpread());
    mRandTp = RandomVal(mOwner->AvgTranspose(), mOwner->TransposeSpread());
}
