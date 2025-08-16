#pragma once
#include "math/Color.h"
#include "obj/ObjPtr_p.h"
#include "obj/Object.h"
#include "rndobj/CubeTex.h"
#include "rndobj/Fur.h"
#include "rndobj/Tex.h"

enum MatProp {
    kMatPropMax = 0x3F
};

enum MatPropEditAction {
};

struct MatPerfSettings {
    MatPerfSettings()
        : mRecvProjLights(false), mRecvPointCubeTex(false), mPS3ForceTrilinear(false) {}

    /** "Check this option to allow the material to receive projected lighting" */
    bool mRecvProjLights;
    /** "Check this option to allow the material to receive projected cube maps from a
     * point light" */
    bool mRecvPointCubeTex;
    /** "Force trilinear filtering of diffuse map (PS3 only)" */
    bool mPS3ForceTrilinear;
};

// size 0x1f8
class BaseMaterial : public Hmx::Object {
public:
    virtual ~BaseMaterial();
    OBJ_CLASSNAME(BaseMaterial);
    OBJ_SET_TYPE(BaseMaterial);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

protected:
    BaseMaterial();

    Hmx::Color mColor; // 0x2c
    bool mUseEnviron; // 0x3c
    bool mPrelit; // 0x3d
    ObjPtr<RndTex> mDiffuseTex; // 0x40
    ObjPtr<RndTex> mDiffuseTex2; // 0x54
    bool mIntensify; // 0x68
    int mTexWrap; // 0x6c
    int mTexGen; // 0x70
    Transform mTexXfm; // 0x74
    int mBlend; // 0xb4
    int mZMode; // 0xb8
    bool mAlphaCut; // 0xbc
    bool mAlphaWrite; // 0xbd
    bool mForceAlphaWrite; // 0xbe
    int mAlphaThreshold; // 0xc0
    ObjPtr<BaseMaterial> mNextPass; // 0xc4
    bool mPointLights; // 0xd8
    bool mFog; // 0xd9
    bool mFadeout; // 0xda
    bool mColorAdjust; // 0xdb
    MatPerfSettings mPerfSettings; // 0xdc
    bool mPerPixelLit; // 0xdf
    float mEmissiveMultiplier; // 0xe0
    ObjPtr<RndTex> mEmissiveMap; // 0xe4
    ObjPtr<RndTex> mNormalMap; // 0xf8
    float mDeNormal; // 0x10c
    ObjPtr<RndTex> mNormDetailMap; // 0x110
    float mNormDetailTiling; // 0x124
    float mNormDetailStrength; // 0x128
    Hmx::Color mSpecularRGB; // 0x12c
    Hmx::Color mSpecular2RGB; // 0x13c
    ObjPtr<RndTex> mSpecularMap; // 0x14c
    float mAnisotropy; // 0x160
    Hmx::Color mRimRGB; // 0x164
    ObjPtr<RndTex> mRimMap; // 0x174
    bool mRimLightUnder; // 0x188
    ObjPtr<RndCubeTex> mEnvironMap; // 0x18c
    bool mEnvironMapFalloff; // 0x1a0
    bool mEnvironMapSpecMask; // 0x1a1
    bool mRefractEnabled; // 0x1a2
    float mRefractStrength; // 0x1a4
    ObjPtr<RndTex> mRefractNormalMap; // 0x1a8
    int mShaderVariation; // 0x1bc
    ObjPtr<RndFur> mFur; // 0x1c0
    bool mNeverFitToSpline; // 0x1d4
    bool mAllowDistortionEffects; // 0x1d5
    float mShockwaveMult; // 0x1d8
    float mWorldProjectionTiling; // 0x1dc
    float mWorldProjectionStartBlend; // 0x1e0
    float mWorldProjectionEndBlend; // 0x1e4
    bool mScreenAligned; // 0x1e8
    int mCull; // 0x1ec
    int mStencilMode; // 0x1f0
    float mBloomMultiplier; // 0x1f4
};
