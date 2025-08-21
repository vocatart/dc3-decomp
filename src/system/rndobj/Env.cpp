#include "rndobj/Env.h"
#include "BoxMap.h"
#include "obj/Object.h"
#include "rndobj/Draw.h"
#include "rndobj/Trans.h"
#include "utl/BinStream.h"

BoxMapLighting RndEnviron::sGlobalLighting;
RndEnviron *RndEnviron::sCurrent;
Vector3 RndEnviron::sCurrentPos;
bool RndEnviron::sCurrentPosSet;

bool RndEnviron::FogEnable() const { return mAmbientFogOwner->mFogEnable; }

Transform RndEnviron::LRFadeRef() const {
    Transform ret;
    if (mFadeRef) {
        ret = mFadeRef->WorldXfm();
    } else {
        ret.Reset();
    }
    return ret;
}

const Transform &RndEnviron::ColorXfm() const {
    static Vector3 x(1, 0, 0);
    static Vector3 y(0, 1, 0);
    static Vector3 z(0, 0, 1);
    static Transform ident(Hmx::Matrix3(x, y, z), Vector3(0, 0, 0));
    if (mUseColorAdjust)
        return mColorXfm.ColorXfm();
    else
        return ident;
}

void RndEnviron::Save(BinStream &bs) {
    bs << 0x10;
    SAVE_SUPERCLASS(Hmx::Object);
    SAVE_SUPERCLASS(RndDrawable);
    SAVE_SUPERCLASS(RndTransformable);
    bs << mLightsReal << mLightsApprox;
}

bool RndEnviron::IsLightInList(const RndLight *light, const ObjPtrList<RndLight> &pList)
    const {
    if (light == nullptr)
        return 0;
    return pList.find(light) != pList.end();
}

bool RndEnviron::IsFake(RndLight *l) const { return IsLightInList(l, mLightsApprox); }
bool RndEnviron::IsReal(RndLight *l) const { return IsLightInList(l, mLightsReal); }

void RndEnviron::RemoveLight(RndLight *l) {
    mLightsReal.remove(l);
    mLightsApprox.remove(l);
}

void RndEnviron::OnRemoveAllLights() {
    mLightsReal.clear();
    mLightsApprox.clear();
    mLightsOld.clear();
}

BEGIN_PROPSYNCS(RndEnviron)
    SYNC_PROP(lights_real, mLightsReal)
    SYNC_PROP(lights_approx, mLightsApprox)
    SYNC_PROP(ambient_color, mAmbientFogOwner->mAmbientColor)
    SYNC_PROP(ambient_alpha, mAmbientFogOwner->mAmbientColor.alpha)
    SYNC_PROP(fog_enable, mAmbientFogOwner->mFogEnable)
    SYNC_PROP(fog_start, mAmbientFogOwner->mFogStart)
    SYNC_PROP(fog_end, mAmbientFogOwner->mFogEnd)
    SYNC_PROP(fog_color, mAmbientFogOwner->mFogColor)
    SYNC_PROP(ambient_fog_owner, mAmbientFogOwner)
    SYNC_PROP(fade_out, mFadeOut)
    SYNC_PROP(fade_start, mFadeStart)
    SYNC_PROP(fade_end, mFadeEnd)
    SYNC_PROP(fade_max, mFadeMax)
    SYNC_PROP(fade_ref, mFadeRef)
    SYNC_PROP(left_out, mLRFade.x)
    SYNC_PROP(left_opaque, mLRFade.y)
    SYNC_PROP(right_opaque, mLRFade.z)
    SYNC_PROP(right_out, mLRFade.w)
    SYNC_PROP(ao_strength, mAOStrength)
    SYNC_PROP(intensity_rate, mIntensityRate)
    SYNC_PROP(exposure, mExposure)
    SYNC_PROP(white_point, mWhitePoint)
    SYNC_PROP(tone_map, mUseToneMapping)
    SYNC_PROP(use_color_adjust, mUseColorAdjust)
    SYNC_PROP_MODIFY(hue, mColorXfm.Hue(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(saturation, mColorXfm.Saturation(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(lightness, mColorXfm.Lightness(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(brightness, mColorXfm.Brightness(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(contrast, mColorXfm.Contrast(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(in_lo, mColorXfm.LevelInLo(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(in_hi, mColorXfm.LevelInHi(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(out_lo, mColorXfm.LevelOutLo(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(out_hi, mColorXfm.LevelOutHi(), mColorXfm.AdjustColorXfm())
    SYNC_PROP(animate_from_preset, mAnimateFromPreset)
    SYNC_SUPERCLASS(RndDrawable)
    SYNC_SUPERCLASS(RndTransformable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

RndEnviron::RndEnviron()
    : mLightsReal(this), mLightsApprox(this), mLightsOld(this), mAmbientColor(0, 0, 0, 1),
      mAmbientFogOwner(this, this), mFogEnable(0), mFogStart(0), mFogEnd(1),
      mFogColor(1, 1, 1), mFadeOut(0), mFadeStart(0), mFadeEnd(1000), mFadeMax(1),
      mFadeRef(this), mLRFade(0, 0, 0, 0), mColorXfm(), mUseColorAdjust(0),
      mAnimateFromPreset(1), mAOEnabled(1), mAOStrength(1), mUpdateTimer(),
      mIntensityAverage(0), mIntensityRate(0.1f), mExposure(1), mWhitePoint(1),
      mUseToneMapping(0), mUseApprox_Local(1), mUseApprox_Global(1) {
    mUpdateTimer.Restart();
}
