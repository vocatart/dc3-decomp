#pragma once
#include "rndobj/Anim.h"
#include "rndobj/PropKeys.h"

/**
 * @brief: A property animator.
 * Original _objects description:
 * "Animate any properties on target object"
 */
class RndPropAnim : public RndAnimatable {
public:
    virtual ~RndPropAnim();
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(PropAnim);
    OBJ_SET_TYPE(PropAnim);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    virtual bool Loop() { return mLoop; }
    virtual void StartAnim();
    virtual void EndAnim() {}
    virtual void SetFrame(float, float);
    virtual float StartFrame();
    virtual float EndFrame();
    virtual void SetKey(float);
    virtual DataNode OnListFlowLabels(DataArray *);

    /** Get the PropKeys from our collection that has the supplied target and prop.
     * @param [in] obj The target object the PropKeys must have.
     * @param [in] prop The property that the PropKeys must be animating.
     * @returns The PropKeys with the above target and prop, if it exists.
     */
    PropKeys *GetKeys(const Hmx::Object *obj, DataArray *prop);

    /** Add a new set of keys to our collection.
     * @param [in] obj The target object for the new keys.
     * @param [in] prop The property of the target object the new keys should animate.
     * @param [in] keysType The new keys' type.
     * @returns The newly created and added keys.
     */
    PropKeys *AddKeys(Hmx::Object *obj, DataArray *prop, PropKeys::AnimKeysType keysType);

protected:
    RndPropAnim();

    /** A collection of PropKeys. */
    std::list<PropKeys *> mPropKeys; // 0x10
    float mLastFrame; // 0x18
    bool mInSetFrame; // 0x1c
    /** "Do I self loop on SetFrame" */
    bool mLoop; // 0x1d
    Symbol unk20;
    float unk24;
    std::list<String> unk28;
};
