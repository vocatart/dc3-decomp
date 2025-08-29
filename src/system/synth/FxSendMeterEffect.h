#pragma once
#include "synth/FxSend.h"
#include "utl/Str.h"
#include <vector>

// courtesy of RB2
struct LevelData {
    float mRMS; // offset 0x0, size 0x4
    float mPeak; // offset 0x4, size 0x4
    float mPeakHold; // offset 0x8, size 0x4
    int mPeakAge; // offset 0xC, size 0x4
    class String mName; // offset 0x10, size 0xC
};

/** "meter effect" */
class FxSendMeterEffect : public FxSend {
public:
    OBJ_CLASSNAME(FxSendMeterEffect);
    OBJ_SET_TYPE(FxSendMeterEffect);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

    OBJ_MEM_OVERLOAD(0x11);

    float ChannelData(int);

protected:
    FxSendMeterEffect();

    bool mResetPeaks; // 0x60
    std::vector<LevelData> mChannels; // 0x64
};
