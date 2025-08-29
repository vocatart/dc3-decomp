#pragma once
#include "obj/Object.h"
#include "synth/Pollable.h"
#include "synth/Faders.h"

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

    float AvgVol() const { return mAvgVol; }
    float VolSpread() const { return mVolSpread; }
    float AvgTranspose() const { return mAvgTranspose; }
    float TransposeSpread() const { return mTransposeSpread; }
    float AvgPan() const { return mAvgPan; }
    float PanSpread() const { return mPanSpread; }

    SeqInst *MakeInst();
    /** "Play the sequence" */
    SeqInst *Play(float, float, float);
    DataNode OnPlay(DataArray *);
    void OnTriggerSound(int);
    /** "Stop all instances of this sequence" */
    void Stop(bool);

protected:
    ObjPtrList<SeqInst> mInsts; // 0x38
    /** "Average volume this sequence will be played at, in dB" */
    float mAvgVol; // 0x4c
    /** "Amount to vary the volume above and below the average, in dB" */
    float mVolSpread; // 0x50
    /** "Average transpose this sequence will be played at, in semitones" */
    float mAvgTranspose; // 0x54
    /** "Amount to vary the transpose, in semitones" */
    float mTransposeSpread; // 0x58
    /** "Average pan to apply to this sequence (-1 - 1)" */
    float mAvgPan; // 0x5c
    /** "Amount to vary the pan" */
    float mPanSpread; // 0x60
    FaderGroup mFaders; // 0x64
    /** "If false, this sequence will play to its end and can't be stopped prematurely"
     */
    bool mCanStop; // 0x7c
};

class SeqInst : public Hmx::Object {
public:
    SeqInst(Sequence *);
    virtual ~SeqInst() {}
    virtual void Stop() = 0;
    virtual bool IsRunning() = 0;
    virtual void UpdateVolume() = 0;
    virtual void SetPan(float) = 0;
    virtual void SetTranspose(float) = 0;
    virtual void Poll() {}
    virtual void StartImpl() = 0;

    void Start();
    void SetVolume(float);
    bool Started() const { return mStarted; }

protected:
    Sequence *mOwner; // 0x2c
    float mRandVol; // 0x30
    float mRandPan; // 0x34
    float mRandTp; // 0x38
    float mVolume; // 0x3c
    bool mStarted; // 0x40
};
