#pragma once
#include "obj/ObjRef.h"
#include "os/Debug.h"
#include "utl/BinStream.h"
#include <cstddef>
#include <vector>

// ObjPtr size: 0x14
template <class T>
class ObjPtr : public ObjRefConcrete<T> {
private:
    Hmx::Object *mOwner; // 0x10
public:
    ObjPtr(Hmx::Object *owner, T *ptr = nullptr)
        : ObjRefConcrete<T>(ptr), mOwner(owner) {}
    ObjPtr(const ObjPtr &p) : ObjRefConcrete(p), mOwner(p.mOwner) {}
    virtual ~ObjPtr() {}
    virtual Hmx::Object *RefOwner() const { return mOwner; }

    void operator=(T *obj) { SetObjConcrete(obj); }
    void operator=(const ObjPtr &p) { CopyRef(p); }
    T *Ptr() const { return mObject; }
};

template <class T1>
BinStream &operator>>(BinStream &bs, ObjPtr<T1> &ptr) {
    ptr.Load(bs, true, nullptr);
    return bs;
}

// ObjOwnerPtr size: 0x14
template <class T>
class ObjOwnerPtr : public ObjRefConcrete<T> {
private:
    ObjRefOwner *mOwner; // 0x10
public:
    ObjOwnerPtr(ObjRefOwner *owner, T *ptr = nullptr)
        : ObjRefConcrete<T>(ptr), mOwner(owner) {
        MILO_ASSERT(owner, 0xC8);
    }
    ObjOwnerPtr(const ObjOwnerPtr &o) : ObjRefConcrete(o.mObject), mOwner(o.mOwner) {
        MILO_ASSERT(mOwner, 0xCE);
    }
    virtual ~ObjOwnerPtr() {}
    virtual Hmx::Object *RefOwner() const { return mObject->RefOwner(); }
    virtual void Replace(Hmx::Object *obj) { mOwner->Replace(this, obj); }
    void operator=(T *obj) { SetObjConcrete(obj); }
    T *Ptr() const { return mObject; }
};

enum EraseMode {
};

enum ObjListMode {
    kObjListNoNull,
    kObjListAllowNull,
    kObjListOwnerControl
};

// ObjPtrVec size: 0x1c
template <class T1, class T2 = class ObjectDir>
class ObjPtrVec : public ObjRefOwner {
    // Node size: 0x14
    struct Node : public ObjRefConcrete<T1, T2> {
        virtual ~Node() {}
        virtual Hmx::Object *RefOwner() const;
        virtual void Replace(Hmx::Object *);
        virtual ObjRefOwner *Parent() const { return unk10; }

        ObjRefOwner *unk10; // 0x10
    };
    virtual Hmx::Object *RefOwner() const { return mOwner; }
    virtual bool Replace(ObjRef *, Hmx::Object *);

public:
    ObjPtrVec(Hmx::Object *owner, EraseMode, ObjListMode);
    virtual ~ObjPtrVec() {}
    bool remove(T1 *);
    void push_back(T1 *);
    // void Set(iterator, T1*);
    bool Load(BinStream &, bool, ObjectDir *);

    // see Draw.cpp for this
    void operator=(const ObjPtrVec &other) {
        if (this != &other) {
            mNodes.clear();
        }
        mNodes.reserve(other.mNodes.size());
    }

private:
    std::vector<Node> mNodes; // 0x4
    Hmx::Object *mOwner; // 0x10
    EraseMode mEraseMode; // 0x14
    ObjListMode mListMode; // 0x18
};

template <class T1>
BinStream &operator<<(BinStream &bs, const ObjPtrVec<T1, ObjectDir> &vec);

template <class T1>
BinStream &operator>>(BinStream &bs, ObjPtrVec<T1, ObjectDir> &vec) {
    vec.Load(bs, true, nullptr);
    return bs;
}

// ObjPtrList size: 0x14
template <class T1, class T2 = class ObjectDir>
class ObjPtrList : public ObjRefOwner {
public:
    ObjPtrList(ObjRefOwner *, ObjListMode);
    virtual ~ObjPtrList() { clear(); }

private:
    // Node size: 0x14
    struct Node : public ObjRefConcrete<T1, T2> {
        virtual ~Node() {}
        virtual Hmx::Object *RefOwner() const;
        virtual void Replace(Hmx::Object *);
        virtual ObjRefOwner *Parent() const { return unk10; }

        T1 *Obj() const { return mObject; }

        ObjRefOwner *unk10; // 0x10
        Node *next; // 0x14
        Node *prev; // 0x18
    };
    int mSize; // 0x4
    Node *mNodes; // 0x8
    ObjRefOwner *mOwner; // 0xc
    ObjListMode mListMode; // 0x10

    virtual Hmx::Object *RefOwner() const;
    virtual bool Replace(ObjRef *, Hmx::Object *);

public:
    class iterator {
    public:
        iterator() : mNode(0) {}
        iterator(Node *node) : mNode(node) {}
        T1 *operator*() { return mNode->Obj(); }

        iterator operator++() {
            mNode = mNode->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator!=(iterator it) { return mNode != it.mNode; }
        bool operator==(iterator it) { return mNode == it.mNode; }
        bool operator!() { return mNode == 0; }

        struct Node *mNode; // 0x0
    };

    void clear() {
        while (mSize != 0)
            pop_back();
    }

    void pop_back() {
        MILO_ASSERT(mNodes != NULL, 0x18B);
        erase(mNodes->prev);
    }

    void push_back(T1 *obj) { insert(end(), obj); }

    iterator find(const Hmx::Object *target) const {
        for (iterator it = begin(); it != end(); ++it) {
            if (*it == target)
                return it;
        }
        return end();
    }

    iterator begin() const { return iterator(mNodes); }
    iterator end() const { return iterator(0); }
    iterator erase(iterator);
    iterator insert(iterator, Hmx::Object *);

    void operator=(const ObjPtrList &list);
    bool remove(T1 *);
    bool Load(BinStream &, bool, ObjectDir *, bool);

private:
    void Link(iterator, Node *);
    Node *Unlink(Node *);
};

template <class T1>
BinStream &operator<<(BinStream &bs, const ObjPtrList<T1, ObjectDir> &list);

template <class T1>
BinStream &operator>>(BinStream &bs, ObjPtrList<T1, ObjectDir> &list) {
    list.Load(bs, true, nullptr, true);
    return bs;
}
