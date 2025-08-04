#pragma once
#include "os/Debug.h"

// forward decs
namespace Hmx {
    class Object;
}
class ObjRef;
class ObjRefOwner;
class ObjectDir;

// ObjRefOwner size: 0x4
class ObjRefOwner {
public:
    ObjRefOwner() {}
    virtual ~ObjRefOwner() {}
    virtual Hmx::Object *RefOwner() const = 0;
    virtual bool Replace(ObjRef *, Hmx::Object *) = 0;
};

// ObjRef size: 0xc
class ObjRef {
protected:
public:
    // seems to be a linked list of an Object's refs
    ObjRef *next; // 0x4
    ObjRef *prev; // 0x8
    ObjRef() {}
    virtual ~ObjRef() {}
    virtual Hmx::Object *RefOwner() const { return nullptr; }
    virtual bool IsDirPtr() { return false; }
    virtual Hmx::Object *GetObj() const {
        MILO_FAIL("calling get obj on abstract ObjRef");
        return nullptr;
    }
    virtual void Replace(Hmx::Object *) {
        MILO_FAIL("calling get obj on abstract ObjRef");
    }
    virtual ObjRefOwner *Parent() const { return nullptr; }

    void Init() { next = prev = this; }
    void ReplaceList(Hmx::Object *obj) {
        for (ObjRef *it = next; it != this; it = next) {
            next->Replace(obj);
            if (this == next) {
                MILO_FAIL("ReplaceList stuck in infinite loop");
            }
        }
    }
    int RefCount() const {
        int count = 0;
        for (ObjRef *it = next; it != this; it = it->next) {
            count++;
        }
        return count;
    }
};

// ObjRefConcrete size: 0x10
template <class T1, class T2 = class ObjectDir>
class ObjRefConcrete : public ObjRef {
protected:
    T1 *mObject; // 0xc
public:
    ObjRefConcrete(T1 *obj) : mObject(obj) {
        if (obj) {
            // prev gets set here too
            // next->prev = this;
        }
    }
    ObjRefConcrete(const ObjRefConcrete &o) : mObject(o.mObject) {
        if (mObject) {
            next = o.next;
            prev = o.prev;
            mObject->mRefs.prev = this;
            prev->next = this;
        }
    }

    virtual ~ObjRefConcrete() {
        if (mObject) {
            prev->next = next;
            next->prev = prev;
        }
    }
    virtual Hmx::Object *GetObj() const { return mObject; }
    virtual void Replace(Hmx::Object *obj) { SetObj(obj); }

    void SetObjConcrete(T1 *);
    void CopyRef(const ObjRefConcrete &);
    Hmx::Object *SetObj(Hmx::Object *);
    bool Load(class BinStream &, bool, ObjectDir *);

    T1 *operator->() const { return mObject; }
    operator T1 *() const { return mObject; }
};
