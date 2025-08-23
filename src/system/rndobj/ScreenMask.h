#pragma once
#include "rndobj/Draw.h"
#include "rndobj/Mat.h"
#include "math/Color.h"
#include "math/Geo.h"

class RndScreenMask : public RndDrawable {
public:
    OBJ_CLASSNAME(ScreenMask);
    OBJ_SET_TYPE(ScreenMask);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void DrawShowing();

protected:
    RndScreenMask();

    ObjPtr<RndMat> mMat; // 0x40
    Hmx::Color mColor; // 0x54
    Hmx::Rect mRect; // 0x64
    bool mUseCamRect; // 0x74
};
