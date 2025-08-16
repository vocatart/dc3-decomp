#include "rndobj/BaseMaterial.h"

BaseMaterial::BaseMaterial()
    : mUseEnviron(true), mPrelit(false), mDiffuseTex(this), mDiffuseTex2(this),
      mIntensify(false), mTexWrap(1), mTexGen(0), mBlend(1), mZMode(1), mAlphaCut(false),
      mAlphaWrite(false), mForceAlphaWrite(false), mAlphaThreshold(0), mNextPass(this),
      mPointLights(false), mFog(false), mFadeout(false), mColorAdjust(false),
      mPerPixelLit(false), mEmissiveMultiplier(1), mEmissiveMap(this), mNormalMap(this),
      mDeNormal(0), mNormDetailMap(this), mNormDetailTiling(1), mNormDetailStrength(0),
      mSpecularRGB(0, 0, 0, 10), mSpecular2RGB(0, 0, 0, 10), mSpecularMap(this),
      mAnisotropy(0), mRimRGB(0, 0, 0, 10), mRimMap(this), mRimLightUnder(false),
      mEnvironMap(this), mEnvironMapFalloff(false), mEnvironMapSpecMask(false),
      mRefractEnabled(false), mRefractStrength(0), mRefractNormalMap(this),
      mShaderVariation(0), mFur(this), mNeverFitToSpline(false),
      mAllowDistortionEffects(true), mShockwaveMult(1), mWorldProjectionTiling(0.125),
      mWorldProjectionStartBlend(0.8), mWorldProjectionEndBlend(0.9),
      mScreenAligned(false), mCull(1), mStencilMode(0), mBloomMultiplier(1) {
    mTexXfm.Reset();
}
