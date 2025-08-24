#pragma once
#include "rndobj/Anim.h"
#include "rndobj/Lit.h"

class RndLightAnim : public RndAnimatable {
public:
    // Hmx::Object
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(LightAnim);
    OBJ_SET_TYPE(LightAnim);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // RndAnimatable
    virtual void SetFrame(float, float);
    virtual float EndFrame();
    virtual Hmx::Object *AnimTarget();
    virtual void SetKey(float);

protected:
    RndLightAnim();

    ObjPtr<RndLight> mLight; // 0x10
    Keys<Hmx::Color, Hmx::Color> mColorKeys; // 0x1c
    ObjOwnerPtr<RndLightAnim> mKeysOwner; // 0x24
};
