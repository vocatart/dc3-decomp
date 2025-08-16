#include "rndobj/MetaMaterial.h"
#include "BaseMaterial.h"
#include "obj/ObjMacros.h"

#define SYNC_MAT_PROP_EDIT_ACTION(s, idx)                                                \
    SYNC_PROP_SET(                                                                       \
        s, unk1f8[idx], SetEditAction((MatProp)idx, (MatPropEditAction)_val.Int())       \
    )

#define SYNC_MAT_PROP(s, member, idx)                                                    \
    SYNC_PROP_SET(                                                                       \
        s##_edit_action,                                                                 \
        unk1f8[idx],                                                                     \
        SetEditAction((MatProp)idx, (MatPropEditAction)_val.Int())                       \
    )                                                                                    \
    SYNC_PROP(s, member)

BEGIN_PROPSYNCS(MetaMaterial)
    SYNC_PROP_SET(shader_combos, CalcApproxNumShaders(), )
    SYNC_MAT_PROP(color, mColor, 0)
    SYNC_MAT_PROP(alpha, mColor.alpha, 1)
    SYNC_MAT_PROP(use_environ, mUseEnviron, 2)
    SYNC_MAT_PROP(prelit, mPrelit, 3)
    SYNC_MAT_PROP(diffuse_tex, mDiffuseTex, 4)
    SYNC_MAT_PROP(diffuse_tex2, mDiffuseTex2, 57)
    SYNC_MAT_PROP(intensify, mIntensify, 5)
    SYNC_MAT_PROP(tex_wrap, mTexWrap, 6)
    SYNC_MAT_PROP(tex_gen, mTexGen, 7)
    SYNC_MAT_PROP(tex_xfm, mTexXfm, 8)
    SYNC_MAT_PROP(blend, mBlend, 9)
    SYNC_MAT_PROP(z_mode, mZMode, 10)
    SYNC_MAT_PROP(alpha_cut, mAlphaCut, 11)
    SYNC_PROP(alpha_threshold, mAlphaThreshold)
    SYNC_MAT_PROP(alpha_write, mAlphaWrite, 12)
    SYNC_MAT_PROP_EDIT_ACTION(force_alpha_write_edit_action, 58)
    SYNC_PROP(alpha_write, mForceAlphaWrite) // yes, the same symbol was used twice
    SYNC_MAT_PROP(next_pass, mNextPass, 13)
    SYNC_MAT_PROP(point_lights, mPointLights, 14)
    SYNC_MAT_PROP(fog, mFog, 15)
    SYNC_MAT_PROP(fade_out, mFadeout, 16)
    SYNC_MAT_PROP(color_adjust, mColorAdjust, 17)
    SYNC_MAT_PROP_EDIT_ACTION(revc_proj_lights_edit_action, 18)
    SYNC_PROP_SET(
        recv_proj_lights,
        mPerfSettings.mRecvProjLights,
        mPerfSettings.mRecvProjLights = _val.Int() > 0
    )
    SYNC_MAT_PROP_EDIT_ACTION(recv_point_cube_tex_edit_action, 19)
    SYNC_PROP_SET(
        recv_point_cube_tex,
        mPerfSettings.mRecvPointCubeTex,
        mPerfSettings.mRecvPointCubeTex = _val.Int() > 0
    )
    SYNC_MAT_PROP_EDIT_ACTION(ps3_force_trilinear_edit_action, 20)
    SYNC_PROP_SET(
        ps3_force_trilinear,
        mPerfSettings.mPS3ForceTrilinear,
        mPerfSettings.mPS3ForceTrilinear = _val.Int() > 0
    )
    SYNC_MAT_PROP(per_pixel_lit, mPerPixelLit, 21)
    SYNC_MAT_PROP(emissive_multiplier, mEmissiveMultiplier, 22)
    SYNC_MAT_PROP(emissive_map, mEmissiveMap, 23)
    SYNC_MAT_PROP(normal_map, mNormalMap, 24)
    SYNC_MAT_PROP(de_normal, mDeNormal, 25)
    SYNC_MAT_PROP(norm_detail_map, mNormDetailMap, 26)
    SYNC_MAT_PROP(norm_detail_strength, mNormDetailStrength, 27)
    SYNC_MAT_PROP(norm_detail_tiling, mNormDetailTiling, 28)
    SYNC_MAT_PROP(specular_rgb, mSpecularRGB, 29)
    SYNC_MAT_PROP(specular_power, mSpecularRGB.alpha, 30)
    SYNC_MAT_PROP(specular_map, mSpecularMap, 31)
    SYNC_MAT_PROP(anisotropy, mAnisotropy, 32)
    SYNC_MAT_PROP(rim_rgb, mRimRGB, 33)
    SYNC_MAT_PROP(rim_power, mRimRGB.alpha, 34)
    SYNC_MAT_PROP(rim_map, mRimMap, 35)
    SYNC_MAT_PROP(rim_light_under, mRimLightUnder, 36)
    SYNC_MAT_PROP(environ_map, mEnvironMap, 37)
    SYNC_MAT_PROP(environ_map_falloff, mEnvironMapFalloff, 38)
    SYNC_MAT_PROP(environ_map_specmask, mEnvironMapSpecMask, 39)
    SYNC_MAT_PROP(refract_enabled, mRefractEnabled, 40)
    SYNC_MAT_PROP(refract_strength, mRefractStrength, 41)
    SYNC_MAT_PROP(refract_normal_map, mRefractNormalMap, 42)
    SYNC_MAT_PROP(shader_variation, mShaderVariation, 43)
    SYNC_MAT_PROP(specular2_rgb, mSpecular2RGB, 44)
    SYNC_MAT_PROP(specular2_power, mSpecular2RGB.alpha, 45)
    SYNC_MAT_PROP(fur, mFur, 46)
    SYNC_MAT_PROP(screen_aligned, mScreenAligned, 47)
    SYNC_MAT_PROP(cull, mCull, 48)
    SYNC_MAT_PROP(stencil_mode, mStencilMode, 49)
    SYNC_MAT_PROP(bloom_multiplier, mBloomMultiplier, 50)
    SYNC_MAT_PROP(never_fit_to_spline, mNeverFitToSpline, 51)
    SYNC_MAT_PROP(allow_distortion_effects, mAllowDistortionEffects, 52)
    SYNC_MAT_PROP(shockwave_mult, mShockwaveMult, 53)
    SYNC_MAT_PROP(world_projection_tiling, mWorldProjectionTiling, 54)
    SYNC_MAT_PROP(world_projection_start_blend, mWorldProjectionStartBlend, 55)
    SYNC_MAT_PROP(world_projection_end_blend, mWorldProjectionEndBlend, 56)
    SYNC_SUPERCLASS(BaseMaterial)
END_PROPSYNCS
