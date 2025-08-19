#pragma once
#include "math/Color.h"
#include "math/Mtx.h"
#include "math/Vec.h"
#include "obj/Data.h"
#include "obj/Msg.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "rndobj/Trans.h"

class ObjKeys : public Keys<ObjectStage, Hmx::Object *> {
public:
    // ObjKeys(Hmx::Object *o) : mOwner(o) {}
    // Hmx::Object *mOwner; // 0x8

    // // fn_80632140
    // void operator=(const ObjKeys &keys) {
    //     Hmx::Object *oldowner = ObjectStage::sOwner;
    //     if (this != &keys) {
    //         resize(keys.size());
    //         ObjKeys::const_iterator keysit = keys.begin();
    //         for (ObjKeys::iterator it = begin(); it != end(); it++, keysit++) {
    //             *it = *keysit;
    //         }
    //     }
    //     ObjectStage::sOwner = oldowner;
    // }

    // int Add(Hmx::Object *obj, float f, bool b) {
    //     Hmx::Object *oldOwner = ObjectStage::sOwner;
    //     ObjectStage::sOwner = mOwner;
    //     int add = Keys<ObjectStage, Hmx::Object *>::Add(ObjectStage(obj), f, b);
    //     ObjectStage::sOwner = oldOwner;
    //     return add;
    // }
};

class PropKeys : public ObjRefOwner {
public:
    enum AnimKeysType {
        kFloat,
        kColor,
        kObject,
        kBool,
        kQuat,
        kVector3,
        kSymbol
    };

    enum Interpolation {
        kStep,
        kLinear,
        kSpline,
        kSlerp,
        kHermite,
        kEaseIn,
        kEaseOut
    };

    enum ExceptionID {
        kNoException,
        kTransQuat,
        kTransScale,
        kTransPos,
        kDirEvent,
        kHandleInterp,
        kMacro
    };

    PropKeys(Hmx::Object *, Hmx::Object *);
    virtual ~PropKeys();
    virtual Hmx::Object *RefOwner() const { return nullptr; }
    virtual bool Replace(ObjRef *, Hmx::Object *) { return false; }
    /** The first frame of these keys. */
    virtual float StartFrame() { return 0; }
    /** The last frame of these keys. */
    virtual float EndFrame() { return 0; }
    /** Given a supplied index, get the corresponding frame.
     * @param [in] index The index of the keys.
     * @param [out] frame The corresponding frame of the key at the given index.
     * @returns True if the index exists in the keys, false if not.
     */
    virtual bool FrameFromIndex(int index, float &frame) { return false; }
    virtual void SetFrame(float, float, float) {}
    /** Duplicate the key at the given index. */
    virtual void CloneKey(int idx) {}
    virtual int SetKey(float);
    /** Remove the key at the given index.
     * @param [in] idx The index of the key to remove.
     * @returns The new amount of keys.
     */
    virtual int RemoveKey(int idx) { return 0; }
    virtual int RemoveRange(float, float) { return 0; }
    /** Get the number of keys. */
    virtual int NumKeys() { return 0; }
    /** Set the value of the keyframe at the supplied index, to the current value of
     * mTarget's property mProp. */
    virtual void SetToCurrentVal(int idx) {}
    virtual int SetFrameException(float) { return 0; }
    /** Save the keys to a BinStream. */
    virtual void Save(BinStream &);
    /** Load the keys from a BinStream. */
    virtual void Load(BinStreamRev &);
    /** Copy the supplied PropKeys metadata into this. */
    virtual void Copy(const PropKeys *);
    /** Get these keys, as a collection of float keys. */
    virtual Keys<float, float> *AsFloatKeys() {
        MILO_ASSERT(false, 0xAB);
        return nullptr;
    }
    /** Get these keys, as a collection of color keys. */
    virtual Keys<Hmx::Color, Hmx::Color> *AsColorKeys() {
        MILO_ASSERT(false, 0xAD);
        return nullptr;
    }
    /** Get these keys, as a collection of Object keys. */
    virtual ObjKeys *AsObjectKeys() {
        MILO_ASSERT(false, 0xAF);
        return nullptr;
    }
    /** Get these keys, as a collection of bool keys. */
    virtual Keys<bool, bool> *AsBoolKeys() {
        MILO_ASSERT(false, 0xB1);
        return nullptr;
    }
    /** Get these keys, as a collection of Quat keys. */
    virtual Keys<Hmx::Quat, Hmx::Quat> *AsQuatKeys() {
        MILO_ASSERT(false, 0xB3);
        return nullptr;
    }
    /** Get these keys, as a collection of Vector3 keys. */
    virtual Keys<Vector3, Vector3> *AsVector3Keys() {
        MILO_ASSERT(false, 0xB5);
        return nullptr;
    }
    /** Get these keys, as a collection of Symbol keys. */
    virtual Keys<Symbol, Symbol> *AsSymbolKeys() {
        MILO_ASSERT(false, 0xB7);
        return nullptr;
    }
    /** Get the float value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int FloatAt(float frame, float &val) {
        MILO_ASSERT(false, 0xBA);
        return -1;
    }
    /** Get the color value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int ColorAt(float frame, Hmx::Color &val) {
        MILO_ASSERT(false, 0xBC);
        return -1;
    }
    /** Get the Object value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int ObjectAt(float frame, Hmx::Object *&val) {
        MILO_ASSERT(false, 0xBE);
        return -1;
    }
    /** Get the bool value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int BoolAt(float frame, bool &val) {
        MILO_ASSERT(false, 0xC0);
        return -1;
    }
    /** Get the quat value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int QuatAt(float frame, Hmx::Quat &val) {
        MILO_ASSERT(false, 0xC2);
        return -1;
    }
    /** Get the Vector3 value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int Vector3At(float frame, Vector3 &val) {
        MILO_ASSERT(false, 0xC4);
        return -1;
    }
    /** Get the Symbol value associated with the supplied frame.
     * @param [in] frame The keyframe to get a value from.
     * @param [out] val The retrieved value.
     * @returns The index in the vector where this keyframe resides.
     */
    virtual int SymbolAt(float frame, Symbol &val) {
        MILO_ASSERT(false, 0xC6);
        return -1;
    }

    void Print();
    void SetPropExceptionID();
    void SetInterpHandler(Symbol);
    void SetProp(DataNode &);
    void SetTarget(Hmx::Object *);
    void ReSort();
    void ChangeFrame(int idx, float new_frame, bool sort);

    static ExceptionID PropExceptionID(Hmx::Object *, DataArray *);
    static Message sInterpMessage;

protected:
    /** The target object to animate properties on. */
    ObjOwnerPtr<Hmx::Object> mTarget; // 0x4
    /** The property of the target object to animate. */
    DataArray *mProp; // 0x18
    /** The animation keys type. */
    AnimKeysType mKeysType; // 0x1c
    /** The key interpolation type. */
    Interpolation mInterpolation; // 0x20
    ExceptionID mPropExceptionID; // 0x24
    RndTransformable *mTrans; // 0x28
    /** The handler name of any propagated interp messages. */
    Symbol mInterpHandler; // 0x2c
    /** The index of the last keyframe that was modified. */
    unsigned int mLastKeyFrameIndex; // 0x30
    bool unk34; // 0x34
};
