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
    friend class Hmx::Object;

protected:
    // seems to be a linked list of an Object's refs
    ObjRef *next; // 0x4
    ObjRef *prev; // 0x8

    // i *think* this is good?
    void AddRef(ObjRef *ref) {
        next = ref;
        prev = ref->prev;
        ref->prev = this;
        prev->next = this;
    }

    void Release(ObjRef *ref) {
        prev->next = next;
        next->prev = prev;
        // do something with ref here
    }

public:
    ObjRef() {}
    ObjRef(const ObjRef &other) : next(other.next), prev(other.prev) {
        prev->next = this;
        next->prev = this;
    }
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

    class iterator {
    private:
        ObjRef *curRef;

    public:
        iterator() : curRef(nullptr) {}
        iterator(ObjRef *ref) : curRef(ref) {}
        operator ObjRef *() const { return curRef; }
        ObjRef *operator->() const { return curRef; }

        iterator operator++() {
            curRef = curRef->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator!=(iterator it) { return curRef != it.curRef; }
        bool operator==(iterator it) { return curRef == it.curRef; }
        bool operator!() { return curRef == nullptr; }
    };

    iterator begin() const { return iterator(next); }
    iterator end() const { return iterator((ObjRef *)this); }

    void Clear() { next = prev = this; }
    void ReplaceList(Hmx::Object *obj) {
        while (next != this) {
            next->Replace(obj);
            if (this == next) {
                MILO_FAIL("ReplaceList stuck in infinite loop");
            }
        }
    }
    int RefCount() const {
        int count = 0;
        for (ObjRef::iterator it = begin(); it != end(); ++it) {
            count++;
        }
        return count;
    }

    // per ObjectDir::HasDirPtrs, this is the way to iterate across refs
    // for (ObjRef *it = mRefs.next; it != &mRefs; it = it->next) {
};

// ObjRefConcrete size: 0x10
template <class T1, class T2 = class ObjectDir>
class ObjRefConcrete : public ObjRef {
protected:
    T1 *mObject; // 0xc
public:
    ObjRefConcrete(T1 *obj) : mObject(obj) {
        if (mObject) {
            mObject->AddRef(this);
        }
    }

    ObjRefConcrete(const ObjRefConcrete &o) : mObject(o.mObject) {
        if (mObject) {
            mObject->AddRef(this);
        }
    }

    virtual ~ObjRefConcrete() {
        if (mObject) {
            mObject->Release(this);
        }
    }
    virtual Hmx::Object *GetObj() const { return mObject; }
    virtual void Replace(Hmx::Object *obj) { SetObj(obj); }

    void SetObjConcrete(T1 *obj) {
        if (mObject) {
            mObject->Release(this);
        }
        mObject = obj;
        if (mObject) {
            mObject->AddRef(this);
        }
    }

    void CopyRef(const ObjRefConcrete &);
    Hmx::Object *SetObj(Hmx::Object *root_obj) {
        T1 *obj = root_obj ? dynamic_cast<T1 *>(root_obj) : nullptr;
        SetObjConcrete(obj);
        return mObject ? mObject : nullptr;
    }
    bool Load(class BinStream &, bool, ObjectDir *);

    T1 *operator->() const { return mObject; }
    operator T1 *() const { return mObject; }
};

#include "utl/BinStream.h"

template <class T1>
BinStream &operator<<(BinStream &bs, const ObjRefConcrete<T1, class ObjectDir> &f) {
    MILO_ASSERT(f.RefOwner(), 0x4D1);
    const char *objName = f ? f->Name() : "";
    bs << objName;
    return bs;
}
