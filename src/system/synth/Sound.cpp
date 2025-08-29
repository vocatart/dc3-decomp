#include "synth/Sound.h"
#include "SynthSample.h"
#include "math/Utl.h"
#include "obj/Object.h"
#include "synth/MoggClip.h"
#include "synth/Synth.h"
#include "synth/Utl.h"

BEGIN_PROPSYNCS(Sound)
    SYNC_SUPERCLASS(Hmx::Object)
    SYNC_PROP(faders, mFaders)
    SYNC_PROP(duckers, mDuckers)
    SYNC_PROP(envelope, mEnvelope)
    SYNC_PROP(loop, mLoop)
    SYNC_PROP(loop_start, mLoopStart)
    SYNC_PROP(loop_end, mLoopEnd)
    SYNC_PROP(max_polyphony, mMaxPolyphony)
    SYNC_PROP_SET(volume, mVolume, SetVolume(_val.Float(), nullptr))
    SYNC_PROP_SET(
        transpose,
        CalcTransposeFromSpeed(mSpeed),
        SetSpeed(CalcSpeedFromTranspose(_val.Float()), nullptr)
    )
    SYNC_PROP_SET(pan, mPan, SetPan(_val.Float(), nullptr))
    SYNC_PROP_SET(send, mSend.Ptr(), SetSend(_val.Obj<FxSend>()))
    SYNC_PROP_SET(reverb_mix_db, mReverbMixDb, SetReverbMixDb(_val.Float()))
    SYNC_PROP_SET(reverb_enable, mReverbEnable, SetReverbEnable(_val.Int()))
    SYNC_PROP_SET(sample, mSynthSample.Ptr(), SetSynthSample(_val.Obj<SynthSample>()))
    SYNC_PROP_SET(mogg, mMoggClip.Ptr(), SetMoggClip(_val.Obj<MoggClip>()))
    SYNC_PROP_SET(trigger_sound, 0, OnTriggerSound(_val.Int()))
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

Sound::Sound()
    : mVolume(0), mSpeed(1), mPan(0), mSend(this), mReverbMixDb(kDbSilence),
      mReverbEnable(0), unk3d(1), mSynthSample(this), mMoggClip(this), mEnvelope(this),
      mFaders(this), mDuckers(this), mLoop(0), mLoopStart(0), mLoopEnd(-1),
      mMaxPolyphony(0), unkb4(0), unkb8(this) {
    mFaders.Add(TheSynth->MasterFader());
}
