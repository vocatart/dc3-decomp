#pragma once
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "rndobj/Trans.h"

class RndTransAnim : public RndAnimatable {
public:
    // Hmx::Object
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(TransAnim);
    OBJ_SET_TYPE(TransAnim);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    // RndAnimatable
    virtual void SetFrame(float, float);
    virtual float StartFrame();
    virtual float EndFrame();
    virtual Hmx::Object *AnimTarget();
    virtual void SetKey(float);

protected:
    RndTransAnim();

    /** The Trans to animate. */
    ObjPtr<RndTransformable> mTrans; // 0x10
    /** Whether or not to use spline interpolation for the trans keys. */
    bool mTransSpline; // 0x1C
    /** Whether or not to use spline interpolation for the scale keys. */
    bool mScaleSpline; // 0x1D
    /** Whether or not to use slerp interpolation for the rot keys. */
    bool mRotSlerp; // 0x1E
    /** Whether or not to use spline interpolation for the rot keys. */
    bool mRotSpline; // 0x1F
    /** The collection of rotation keys. */
    Keys<Hmx::Quat, Hmx::Quat> mRotKeys; // 0x20
    /** The collection of position keys. */
    Keys<Vector3, Vector3> mTransKeys; // 0x28
    /** The collection of scale keys. */
    Keys<Vector3, Vector3> mScaleKeys; // 0x30
    /** The TransAnim that owns all of these keys. */
    ObjOwnerPtr<RndTransAnim> mKeysOwner; // 0x38
    bool mRepeatTrans; // 0x44
    bool mFollowPath; // 0x45
};
