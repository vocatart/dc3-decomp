#pragma once
#include "rndobj/Anim.h"
#include "rndobj/Mat.h"
#include "rndobj/Tex.h"

class RndMatAnim : public RndAnimatable {
public:
    class TexPtr : public ObjPtr<RndTex> {
    public:
        TexPtr();
        TexPtr(RndTex *);
    };

    class TexKeys : public Keys<TexPtr, RndTex *> {
    public:
        TexKeys(Hmx::Object *owner) : mOwner(owner) {}
        TexKeys &operator=(const TexKeys &);
        int Add(RndTex *, float, bool);

        Hmx::Object *mOwner;
    };

    // Hmx::Object
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(MatAnim);
    OBJ_SET_TYPE(MatAnim);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    // RndAnimatable
    virtual void SetFrame(float, float);
    virtual float EndFrame();
    virtual Hmx::Object *AnimTarget();
    virtual void SetKey(float);

protected:
    RndMatAnim();

    /** The Mat to animate. */
    ObjPtr<RndMat> mMat; // 0x10
    /** The MatAnim that owns all of these keys. */
    ObjOwnerPtr<RndMatAnim> mKeysOwner; // 0x1c
    /** The collection of Color keys. */
    Keys<Hmx::Color, Hmx::Color> mColorKeys; // 0x28
    /** The collection of alpha keys. */
    Keys<float, float> mAlphaKeys; // 0x30
    /** The collection of position keys. */
    Keys<Vector3, Vector3> mTransKeys; // 0x38
    /** The collection of scale keys. */
    Keys<Vector3, Vector3> mScaleKeys; // 0x40
    /** The collection of rotation keys. */
    Keys<Vector3, Vector3> mRotKeys; // 0x48
    /** The collection of texture keys. */
    TexKeys mTexKeys; // 0x50
};
