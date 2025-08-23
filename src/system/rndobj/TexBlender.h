#pragma once
#include "rndobj/Draw.h"
#include "rndobj/Tex.h"
#include "rndobj/TexBlendController.h"

class RndTexBlender : public RndDrawable {
public:
    // Hmx::Object
    virtual ~RndTexBlender() {}
    OBJ_CLASSNAME(TexBlender);
    OBJ_SET_TYPE(TexBlender);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    // RndDrawable
    virtual float GetDistanceToPlane(const Plane &, Vector3 &);
    virtual bool MakeWorldSphere(Sphere &, bool);
    virtual void DrawShowing();

protected:
    RndTexBlender();

    ObjPtr<RndTex> mBaseMap; // 0x40
    ObjPtr<RndTex> mNearMap; // 0x54
    ObjPtr<RndTex> mFarMap; // 0x68
    ObjPtr<RndTex> mOutputTextures; // 0x7c
    ObjPtrList<RndTexBlendController> mControllerList; // 0x90
    ObjPtr<RndDrawable> mOwner; // 0xa4
    float mControllerInfluence; // 0xb8
    int unkbc; // 0xbc
    bool unkc0;
};
