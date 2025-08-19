#include "rndobj/PostProc.h"
#include "obj/Object.h"
#include "os/System.h"
#include "rndobj/DOFProc.h"
#include "utl/BinStream.h"

void RndPostProc::ResetDofProc() { TheDOFProc->UnSet(); }
RndPostProc *RndPostProc::Current() { return sCurrent; }

ProcCounter::ProcCounter()
    : mProcAndLock(0), mCount(0), mSwitch(0), mOdd(0), mFPS(0), mEvenOddDisabled(0),
      mTriFrameRendering(0) {}

void ProcCounter::SetProcAndLock(bool pandl) {
    mProcAndLock = pandl;
    mCount = -1;
}

void ProcCounter::SetEvenOddDisabled(bool eod) {
    if (mEvenOddDisabled == eod)
        return;
    else
        mEvenOddDisabled = eod;
    if (mEvenOddDisabled)
        mCount = -1;
}

void RndPostProc::Init() {
    sBloomLocFactor = SystemConfig("rnd", "bloom_loc")->FindFloat(SystemLanguage());
}

void RndPostProc::Reset() {
    if (sCurrent) {
        sCurrent->OnUnselect();
        sCurrent = nullptr;
    }
    TheDOFProc->UnSet();
}

void RndPostProc::Save(BinStream &bs) {
    bs << packRevs(2, 0x25);
    SAVE_SUPERCLASS(Hmx::Object)
    bs << mBloomColor;
    bs << mBloomIntensity;
    bs << mBloomThreshold;
    mColorXfm.Save(bs);
    bs << mFlickerModBounds << mFlickerTimeBounds;
    bs << mNoiseBaseScale << mNoiseTopScale << mNoiseIntensity << mNoiseStationary;
    bs << mNoiseMap;
    bs << mNoiseMidtone;
    bs << mTrailThreshold;
    bs << mTrailDuration;
    bs << mEmulateFPS;
    bs << mPosterLevels;
    bs << mPosterMin;
    bs << mKaleidoscopeComplexity;
    bs << mKaleidoscopeSize;
    bs << mKaleidoscopeAngle;
    bs << mKaleidoscopeRadius;
    bs << mKaleidoscopeFlipUVs;
    bs << mHallOfTimeRate;
    bs << mHallOfTimeColor << mHallOfTimeMix;
    bs << mHallOfTimeType;
    bs << mMotionBlurBlend;
    bs << mMotionBlurWeight;
    bs << mMotionBlurVelocity;
    bs << mGradientMap;
    bs << mGradientMapOpacity;
    bs << mGradientMapIndex;
    bs << mGradientMapStart;
    bs << mGradientMapEnd;
    bs << mRefractMap;
    bs << mRefractDist;
    bs << mRefractScale;
    bs << mRefractPanning;
    bs << mRefractAngle;
    bs << mRefractVelocity;
    bs << mChromaticAberrationOffset;
    bs << mChromaticSharpen;
    bs << mVignetteColor;
    bs << mVignetteIntensity;
    bs << mBloomGlare;
    bs << mBloomStreak;
    bs << mBloomStreakAttenuation;
    bs << mBloomStreakAngle;
    bs << mHueTarget;
    bs << mHueFocus;
    bs << mBlendAmount;
    bs << mBrightnessPower;
}

RndPostProc::RndPostProc()
    : mPriority(1), mBloomColor(1, 1, 1, 0), mBloomThreshold(4), mBloomIntensity(0),
      mBloomGlare(0), mBloomStreak(0), mBloomStreakAttenuation(0.9f),
      mBloomStreakAngle(0), mForceCurrentInterp(0), mColorXfm(), mPosterLevels(0),
      mPosterMin(1), mKaleidoscopeComplexity(0), mKaleidoscopeSize(0.5f),
      mKaleidoscopeAngle(0), mKaleidoscopeRadius(0), mKaleidoscopeFlipUVs(1),
      mFlickerModBounds(0, 0), mFlickerTimeBounds(0.001f, 0.007f), mFlickerSeconds(0, 0),
      mColorModulation(1), mNoiseBaseScale(32, 24), mNoiseTopScale(1.35914f),
      mNoiseIntensity(0), mNoiseStationary(0), mNoiseMidtone(1), mNoiseMap(this, 0),
      mTrailThreshold(1), mTrailDuration(0), mBlendVec(1, 1, 0), mEmulateFPS(30),
      mLastRender(0), mHallOfTimeType(0), mHallOfTimeRate(0),
      mHallOfTimeColor(1, 1, 1, 0), mHallOfTimeMix(0), mMotionBlurWeight(1, 1, 1, 0),
      mMotionBlurBlend(0), mMotionBlurVelocity(1), mGradientMap(this, 0),
      mGradientMapOpacity(0), mGradientMapIndex(0), mGradientMapStart(0),
      mGradientMapEnd(1), mRefractMap(this, 0), mRefractDist(0.05f), mRefractScale(1, 1),
      mRefractPanning(0, 0), mRefractVelocity(0, 0), mRefractAngle(0),
      mChromaticAberrationOffset(0), mChromaticSharpen(0), mVignetteColor(0, 0, 0, 0),
      mVignetteIntensity(0), mHueTarget(-75), mHueFocus(0.958), mBlendAmount(0),
      mBrightnessPower(1) {
    mColorXfm.Reset();
}

BEGIN_HANDLERS(RndPostProc)
    HANDLE_SUPERCLASS(Hmx::Object)
    HANDLE_ACTION(select, Select())
    HANDLE_ACTION(unselect, Unselect())
    HANDLE_ACTION(multi_select, OnSelect())
    HANDLE_ACTION(multi_unselect, OnUnselect())
    HANDLE_ACTION(
        interp,
        Interp(_msg->Obj<RndPostProc>(2), _msg->Obj<RndPostProc>(3), _msg->Float(4))
    )
    HANDLE(allowed_normal_map, OnAllowedNormalMap)
END_HANDLERS

BEGIN_PROPSYNCS(RndPostProc)
    SYNC_PROP(priority, mPriority)
    SYNC_PROP(bloom_color, mBloomColor)
    SYNC_PROP(bloom_threshold, mBloomThreshold)
    SYNC_PROP(bloom_intensity, mBloomIntensity)
    SYNC_PROP(bloom_glare, mBloomGlare)
    SYNC_PROP(bloom_streak, mBloomStreak)
    SYNC_PROP(bloom_streak_attenuation, mBloomStreakAttenuation)
    SYNC_PROP(bloom_streak_angle, mBloomStreakAngle)
    SYNC_PROP_MODIFY(hue, mColorXfm.Hue(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(saturation, mColorXfm.Saturation(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(lightness, mColorXfm.Lightness(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(brightness, mColorXfm.Brightness(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(contrast, mColorXfm.Contrast(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(in_lo, mColorXfm.LevelInLo(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(in_hi, mColorXfm.LevelInHi(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(out_lo, mColorXfm.LevelOutLo(), mColorXfm.AdjustColorXfm())
    SYNC_PROP_MODIFY(out_hi, mColorXfm.LevelOutHi(), mColorXfm.AdjustColorXfm())
    SYNC_PROP(num_levels, mPosterLevels)
    SYNC_PROP(min_intensity, mPosterMin)
    SYNC_PROP(kaleidoscope_complexity, mKaleidoscopeComplexity)
    SYNC_PROP(kaleidoscope_size, mKaleidoscopeSize)
    SYNC_PROP(kaleidoscope_angle, mKaleidoscopeAngle)
    SYNC_PROP(kaleidoscope_radius, mKaleidoscopeRadius)
    SYNC_PROP(kaleidoscope_flipUVs, mKaleidoscopeFlipUVs)
    SYNC_PROP(flicker_intensity, mFlickerModBounds)
    SYNC_PROP(flicker_secs_range, mFlickerTimeBounds)
    SYNC_PROP(noise_base_scale, mNoiseBaseScale)
    SYNC_PROP(noise_intensity, mNoiseIntensity)
    SYNC_PROP(noise_stationary, mNoiseStationary)
    SYNC_PROP(noise_midtone, mNoiseMidtone)
    SYNC_PROP(noise_map, mNoiseMap)
    SYNC_PROP(threshold, mTrailThreshold)
    SYNC_PROP(duration, mTrailDuration)
    SYNC_PROP(emulate_fps, mEmulateFPS)
    SYNC_PROP(hall_of_time_type, mHallOfTimeType)
    SYNC_PROP(hall_of_time_rate, mHallOfTimeRate)
    SYNC_PROP(hall_of_time_color, mHallOfTimeColor)
    SYNC_PROP(hall_of_time_mix, mHallOfTimeMix)
    SYNC_PROP(motion_blur_blend, mMotionBlurBlend)
    SYNC_PROP(motion_blur_weight, mMotionBlurWeight)
    SYNC_PROP(motion_blur_exposure, mMotionBlurWeight.alpha)
    SYNC_PROP(motion_blur_velocity, mMotionBlurVelocity)
    SYNC_PROP(gradient_map, mGradientMap)
    SYNC_PROP(gradient_map_opacity, mGradientMapOpacity)
    SYNC_PROP(gradient_map_index, mGradientMapIndex)
    SYNC_PROP(gradient_map_start, mGradientMapStart)
    SYNC_PROP(gradient_map_end, mGradientMapEnd)
    SYNC_PROP(refract_map, mRefractMap)
    SYNC_PROP(refract_dist, mRefractDist)
    SYNC_PROP(refract_scale, mRefractScale)
    SYNC_PROP(refract_panning, mRefractPanning)
    SYNC_PROP(refract_velocity, mRefractVelocity)
    SYNC_PROP(refract_angle, mRefractAngle)
    SYNC_PROP(chromatic_aberration_offset, mChromaticAberrationOffset)
    SYNC_PROP(chromatic_sharpen, mChromaticSharpen)
    SYNC_PROP(vignette_color, mVignetteColor)
    SYNC_PROP(vignette_intensity, mVignetteIntensity)
    SYNC_PROP(hue_target, mHueTarget)
    SYNC_PROP(hue_focus, mHueFocus)
    SYNC_PROP(blend_amount, mBlendAmount)
    SYNC_PROP(brightness_power, mBrightnessPower)
    SYNC_PROP(force_current_interp, mForceCurrentInterp)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
