#include "rndobj/Mat_NG.h"
#include "BaseMaterial.h"
#include "rndobj/BaseMaterial.h"

NgMat *NgMat::sCurrent;

bool NgMat::AllowFog() const {
    return mBlend != kBlendDest && mBlend != kBlendAdd && mBlend != kBlendSubtract
        && mBlend != kBlendSrcAlphaAdd;
}

bool NgMat::AllowHDR() const {
    return (mBlend != kBlendSrcAlpha && mBlend != kBlendSrcAlphaAdd
            && mBlend != kPreMultAlpha)
        && !mAlphaCut && !mAlphaWrite;
}

NgMat::NgMat() {}

void NgMat::SetupShader(bool b1, bool b2) {
    if (b2)
        SetupAmbient();
    if (this != sCurrent || mDirty) {
        if (mDirty & 2) {
            RefreshState();
        }
        SetBasicState();
        if (b2) {
            SetRegularShaderConst(b1);
        }
        mDirty = 0;
        sCurrent = this;
    }
}
