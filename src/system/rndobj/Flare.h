#pragma once
#include "rndobj/Draw.h"
#include "rndobj/Mat.h"
#include "rndobj/Trans.h"
#include "utl/MemMgr.h"

class RndFlare : public RndTransformable, public RndDrawable {
public:
    // Hmx::Object
    virtual ~RndFlare();
    OBJ_CLASSNAME(Flare);
    OBJ_SET_TYPE(Flare);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    // RndDrawable
    virtual void Mats(std::list<class RndMat *> &, bool);
    virtual void DrawShowing();
    virtual void Highlight() { RndDrawable::Highlight(); }

    OBJ_MEM_OVERLOAD(0x18);

protected:
    RndFlare();

    bool mPointTest; // 0x100
    bool mAreaTest; // 0x101
    bool mVisible; // 0x102
    Vector2 mSizes; // 0x104
    ObjPtr<RndMat> mMat; // 0x10C
    Vector2 mRange; // 0x120
    float mOffset; // 0x128
    int mSteps; // 0x12c
    int mStep; // 0x130
    Hmx::Rect mArea; // 0x134
    float unk144; // 0x144
    bool unk148; // 0x148
    bool unk149; // 0x149
    Hmx::Matrix3 mMatrix; // 0x14c
    Vector2 unk17c; // 0x17c
};
