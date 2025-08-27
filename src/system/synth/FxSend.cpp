#include "synth/FxSend.h"
#include "math/Utl.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "utl/BinStream.h"

void FxSend::SetChannels(SendChannels chans) {
    if (chans != mChannels) {
        mChannels = chans;
        RebuildChain();
    }
}

void FxSend::EnableUpdates(bool b) {
    mEnableUpdates = b;
    if (mEnableUpdates)
        OnParametersChanged();
}

bool FxSend::CheckChain(FxSend *send, int i) {
    FxSend *cur;
    for (cur = send; cur && cur != this; cur = cur->mNextSend)
        ;
    if (cur == this) {
        MILO_NOTIFY("Error: can't have loops in your FX chain.");
        return false;
    } else if (send && send->Stage() <= i) {
        MILO_NOTIFY(
            "Error: output send must be set to a higher stage (%d <= %d).",
            send->Stage(),
            i
        );
        return false;
    } else {
        for (ObjRef::iterator it = mRefs.begin(); it != mRefs.end(); ++it) {
            FxSend *rsend = dynamic_cast<FxSend *>((*it).RefOwner());
            if (rsend && rsend->NextSend() == this && rsend->Stage() >= i) {
                MILO_NOTIFY(
                    "Error: stage must be higher than all input sends' stages (see %s).",
                    rsend->Name()
                );
                return false;
            }
        }
        return true;
    }
}

void FxSend::SetStage(int stage) {
    if (stage != mStage && CheckChain(mNextSend, stage)) {
        mStage = stage;
        RebuildChain();
    }
}

void FxSend::Save(BinStream &bs) {
    bs << packRevs(8, 7);
    SAVE_SUPERCLASS(Hmx::Object)
    bs << mNextSend;
    bs << mStage;
    bs << mChannels;
    bs << mDryGain;
    bs << mWetGain;
    bs << mInputGain;
    bs << mBypass;
    bs << mReverbMixDb;
    bs << mReverbEnable;
}

BEGIN_HANDLERS(FxSend)
    HANDLE_ACTION(test_with_mic, TestWithMic())
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

BEGIN_PROPSYNCS(FxSend)
    SYNC_PROP_SET(next_send, NextSend(), SetNextSend(_val.Obj<FxSend>()))
    SYNC_PROP_SET(stage, Stage(), SetStage(_val.Int()))
    SYNC_PROP_MODIFY(dry_gain, mDryGain, UpdateMix())
    SYNC_PROP_MODIFY(wet_gain, mWetGain, UpdateMix())
    SYNC_PROP_MODIFY(input_gain, mInputGain, UpdateMix())
    SYNC_PROP_MODIFY(reverb_mix_db, mReverbMixDb, UpdateMix())
    SYNC_PROP_MODIFY(reverb_enable, mReverbEnable, RebuildChain())
    SYNC_PROP_MODIFY(channels, (int &)mChannels, RebuildChain())
    SYNC_PROP_MODIFY(bypass, mBypass, UpdateMix())
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

FxSend::FxSend()
    : mNextSend(this), mStage(0), mBypass(0), mDryGain(kDbSilence), mWetGain(0),
      mInputGain(0), mReverbMixDb(kDbSilence), mReverbEnable(0), mEnableUpdates(1),
      mChannels(kSendAll) {}

bool FxSend::Replace(ObjRef *ref, Hmx::Object *obj) {
    if (this == mNextSend) {
        mNextSend.SetObj(obj);
        RebuildChain();
        return true;
    } else
        return Hmx::Object::Replace(ref, obj);
}

void FxSend::SetNextSend(FxSend *next) {
    if (next != mNextSend && CheckChain(next, mStage)) {
        mNextSend = next;
        RebuildChain();
    }
}

void FxSend::RebuildChain() {
    std::vector<FxSend *> vec;
    BuildChainVector(vec);
    Recreate(vec);
}

BEGIN_COPYS(FxSend)
    COPY_SUPERCLASS(Hmx::Object)
    CREATE_COPY(FxSend)
    BEGIN_COPYING_MEMBERS
        COPY_MEMBER(mNextSend)
        COPY_MEMBER(mStage)
        COPY_MEMBER(mWetGain)
        COPY_MEMBER(mDryGain)
        COPY_MEMBER(mInputGain)
        COPY_MEMBER(mChannels)
        COPY_MEMBER(mBypass)
        COPY_MEMBER(mReverbMixDb)
        COPY_MEMBER(mReverbEnable)
    END_COPYING_MEMBERS
END_COPYS
