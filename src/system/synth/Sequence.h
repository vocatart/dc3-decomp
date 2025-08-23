#pragma once
#include "obj/Object.h"
#include "synth/Pollable.h"

class SeqInst;

/** "A set of audio tasks" */
class Sequence : public Hmx::Object, public SynthPollable {
public:
    Sequence();
    virtual ~Sequence();
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

    virtual const char *GetSoundDisplayName() {
        return MakeString("Sequence: %s", Name());
    }
    virtual SeqInst *MakeInstImpl() = 0;
    virtual void SynthPoll();

    static void Init();

    SeqInst *MakeInst();
    /** "Play the sequence" */
    SeqInst *Play(float, float, float);
    DataNode OnPlay(DataArray *);
    void OnTriggerSound(int);
    /** "Stop all instances of this sequence" */
    void Stop(bool);

    // ObjPtrList<SeqInst> mInsts; // 0x28
    // /** "Average volume this sequence will be played at, in dB" */
    // float mAvgVol; // 0x38
    // /** "Amount to vary the volume above and below the average, in dB" */
    // float mVolSpread; // 0x3c
    // /** "Average transpose this sequence will be played at, in semitones" */
    // float mAvgTranspose; // 0x40
    // /** "Amount to vary the transpose, in semitones" */
    // float mTransposeSpread; // 0x44
    // /** "Average pan to apply to this sequence (-1 - 1)" */
    // float mAvgPan; // 0x48
    // /** "Amount to vary the pan" */
    // float mPanSpread; // 0x4c
    // FaderGroup mFaders; // 0x50
    // /** "If false, this sequence will play to its end and can't be stopped prematurely"
    // */ bool mCanStop; // 0x64
};
