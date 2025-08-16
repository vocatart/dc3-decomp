#include "rndobj/BaseMaterial.h"
#include "obj/ObjMacros.h"
#include "obj/Object.h"

BaseMaterial::BaseMaterial()
    : mUseEnviron(true), mPrelit(false), mDiffuseTex(this), mDiffuseTex2(this),
      mIntensify(false), mTexWrap(kTexWrapRepeat), mTexGen(kTexGenNone),
      mBlend(kBlendSrc), mZMode(kZModeNormal), mAlphaCut(false), mAlphaWrite(false),
      mForceAlphaWrite(false), mAlphaThreshold(0), mNextPass(this), mPointLights(false),
      mFog(false), mFadeout(false), mColorAdjust(false), mPerPixelLit(false),
      mEmissiveMultiplier(1), mEmissiveMap(this), mNormalMap(this), mDeNormal(0),
      mNormDetailMap(this), mNormDetailTiling(1), mNormDetailStrength(0),
      mSpecularRGB(0, 0, 0, 10), mSpecular2RGB(0, 0, 0, 10), mSpecularMap(this),
      mAnisotropy(0), mRimRGB(0, 0, 0, 10), mRimMap(this), mRimLightUnder(false),
      mEnvironMap(this), mEnvironMapFalloff(false), mEnvironMapSpecMask(false),
      mRefractEnabled(false), mRefractStrength(0), mRefractNormalMap(this),
      mShaderVariation(kShaderVariationNone), mFur(this), mNeverFitToSpline(false),
      mAllowDistortionEffects(true), mShockwaveMult(1), mWorldProjectionTiling(0.125),
      mWorldProjectionStartBlend(0.8), mWorldProjectionEndBlend(0.9),
      mScreenAligned(false), mCull(kCullRegular), mStencilMode(kStencilIgnore),
      mBloomMultiplier(1) {
    mTexXfm.Reset();
}

BEGIN_PROPSYNCS(BaseMaterial)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
