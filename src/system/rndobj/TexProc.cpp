#include "rndobj/TexProc.h"
#include "Rnd.h"
#include "ShaderMgr.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "rndobj/BaseMaterial.h"
#include "rndobj/Cam.h"
#include "rndobj/Draw.h"
#include "rndobj/Mat.h"
#include "utl/BinStream.h"

float gAmpTemp = 0.3f;
float gFreqTemp = 1.0f;
RndCam *TexProc::mCam;

RndMat *TexProc::SetUpWorkingMat() {
    RndMat *mat = TheShaderMgr.GetWork();
    mat->SetZMode(kZModeDisable);
    mat->SetBlend(BaseMaterial::kBlendSrc);
    mat->SetTexWrap(kTexWrapClamp);
    mat->SetAlphaWrite(true);
    mat->SetAlphaCut(false);
    return mat;
}

void TexProc::UpdatePreClearState() {
    TheRnd.PreClearDrawAddOrRemove(this, mDrawPreClear, false);
}

void TexProc::Save(BinStream &bs) {
    bs << 3;
    SAVE_SUPERCLASS(Hmx::Object)
    SAVE_SUPERCLASS(RndDrawable)
    bs << mInputTex;
    bs << mOutputTex;
    bs << mShaderType;
    bs << mDrawPreClear;
    bs << mFrequency;
    bs << mAmplitude;
    bs << mAmplitudeBump;
    bs << mPhaseVel;
}

DataNode TexProc::OnSetParams(DataArray *a) {
    if (CheckParams(a, true)) {
        if (unk70)
            unk70->Release();
        unk70 = new DataArray(a->Size() - 2);
        SetParams(unk70, a);
    } else {
        MILO_NOTIFY("----- TexProc::OnSetParams() - one or more parameters is invalid");
    }
    return 0;
}

BEGIN_COPYS(TexProc)
    COPY_SUPERCLASS(Hmx::Object)
    COPY_SUPERCLASS(RndDrawable)
    CREATE_COPY(TexProc)
    BEGIN_COPYING_MEMBERS
        COPY_MEMBER(mInputTex)
        COPY_MEMBER(mOutputTex)
        COPY_MEMBER(mShaderType)
        COPY_MEMBER(mDrawPreClear)
        COPY_MEMBER(mFrequency)
        COPY_MEMBER(unk78)
        COPY_MEMBER(mPhase)
        COPY_MEMBER(mPhaseVel)
        COPY_MEMBER(mAmplitude)
        COPY_MEMBER(mAmplitudeBump)
    END_COPYING_MEMBERS
END_COPYS

BEGIN_HANDLERS(TexProc)
    HANDLE_SUPERCLASS(RndDrawable)
    HANDLE_SUPERCLASS(Hmx::Object)
    HANDLE(set_params, OnSetParams)
END_HANDLERS

BEGIN_PROPSYNCS(TexProc)
    SYNC_PROP(input_texture, mInputTex)
    SYNC_PROP(output_texture, mOutputTex)
    SYNC_PROP(shader, (int &)mShaderType)
    SYNC_PROP_MODIFY(draw_pre_clear, mDrawPreClear, UpdatePreClearState())
    SYNC_PROP(frequency, mFrequency)
    SYNC_PROP(amplitude, mAmplitude)
    SYNC_PROP(amplitude_bump, mAmplitudeBump)
    SYNC_PROP(phase, mPhase)
    SYNC_PROP(phase_vel, mPhaseVel)
    SYNC_SUPERCLASS(RndDrawable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

TexProc::TexProc()
    : mInputTex(this), mOutputTex(this), mShaderType(kShaderTwirl), mDrawPreClear(1),
      unk70(0), mFrequency(0), unk78(0), mAmplitude(0), mAmplitudeBump(0), mPhase(0),
      mPhaseVel(0) {}

void TexProc::Init() {
    REGISTER_OBJ_FACTORY(TexProc);
    MILO_ASSERT(!mCam, 0x3A);
    mCam = ObjectDir::Main()->New<RndCam>("[tex proc cam]");
}
