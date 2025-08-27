#pragma once
#include "obj/Object.h"
#include "utl/MemMgr.h"

enum SendChannels {
    kSendAll = 0,
    kSendCenter = 1,
    kSendStereo = 2,
    kSendAllXMix = 3
};

class FxSend : public Hmx::Object {
public:
    // Hmx::Object
    virtual ~FxSend() {}
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(FxSend);
    OBJ_SET_TYPE(FxSend);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // FxSend
    virtual void SetNextSend(FxSend *);
    virtual void Recreate(std::vector<FxSend *> &) {}
    virtual void RebuildChain();
    virtual void BuildChainVector(std::vector<FxSend *> &);
    virtual bool CanPushParameters() { return true; }

    OBJ_MEM_OVERLOAD(0x19);

    FxSend *NextSend() const { return mNextSend; }
    int Stage() const { return mStage; }
    void SetChannels(SendChannels);
    void EnableUpdates(bool);
    void TestWithMic();
    void SetStage(int);

protected:
    FxSend();

    virtual void UpdateMix() {}
    virtual void OnParametersChanged() {}

    bool CheckChain(FxSend *, int);

    /** "The next effect in the chain" */
    ObjOwnerPtr<FxSend> mNextSend; // 0x2c
    /** "The relative order that this send is processed compared to other sends." Ranges
     * from 0 to 9. */
    int mStage; // 0x40
    /** "Bypass the effect and stop it from processing" */
    bool mBypass; // 0x44
    /** "Gain applied to dry signal (dB)". Ranges from -96.0 to 20.0. */
    float mDryGain; // 0x48
    /** "Gain applied to effect output (dB)" Ranges from -96.0 to 20.0. */
    float mWetGain; // 0x4c
    /** "Gain applied to effect input (dB)" Ranges from -96.0 to 20.0. */
    float mInputGain; // 0x50
    /** "Reverb send for this effect". Ranges from -96.0 to 20.0. */
    float mReverbMixDb; // 0x54
    /** "Enable reverb send" */
    bool mReverbEnable; // 0x58
    bool mEnableUpdates; // 0x59
    /** "Which channels the FX applies to" */
    SendChannels mChannels; // 0x5c
};

//   ObjOwnerPtr<>::ObjOwnerPtr<>(this + 0x2c,this,0x0);
//   *(this + 0x40) = 0;
//   this[0x44] = 0x0;
//   this[0x58] = 0x0;
//   *(this + 0x48) = -96.0;
//   this[0x59] = 0x1;
//   *(this + 0x4c) = 0.0;
//   *(this + 0x5c) = 0;
//   *(this + 0x50) = 0.0;
//   *(this + 0x54) = -96.0;
