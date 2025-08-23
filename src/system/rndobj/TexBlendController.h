#pragma once
#include "obj/Object.h"
#include "rndobj/Mesh.h"
#include "rndobj/Tex.h"
#include "rndobj/Trans.h"
#include "utl/MemMgr.h"

class RndTexBlendController : public Hmx::Object {
public:
    virtual ~RndTexBlendController() {}
    OBJ_CLASSNAME(TexBlendController);
    OBJ_SET_TYPE(TexBlendController);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);

    OBJ_MEM_OVERLOAD(0x15);

protected:
    RndTexBlendController();

    ObjPtr<RndMesh> mMesh; // 0x2c
    ObjPtr<RndTransformable> mObject1; // 0x40
    ObjPtr<RndTransformable> mObject2; // 0x54
    float mReferenceDistance; // 0x68
    float mMinDistance; // 0x6c
    float mMaxDistance; // 0x70
    ObjPtr<RndTex> mTex; // 0x74
};
