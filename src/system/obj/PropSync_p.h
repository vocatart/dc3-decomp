#pragma once
#include "math/Key.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "obj/PropSync.h"

// DO NOT try to include this header directly!
// include obj/PropSync.h instead

inline bool PropSync(float &f, DataNode &node, DataArray *prop, int i, PropOp op) {
    MILO_ASSERT(i == prop->Size() && op <= kPropInsert, 0x17);
    if (op == kPropGet)
        node = f;
    else
        f = node.Float();
    return true;
}

inline bool
PropSync(unsigned char &uc, DataNode &node, DataArray *prop, int i, PropOp op) {
    MILO_ASSERT(i == prop->Size() && op <= kPropInsert, 0x21);
    if (op == kPropGet)
        node = uc;
    else
        uc = node.Int();
    return true;
}

inline bool PropSync(int &iref, DataNode &node, DataArray *prop, int i, PropOp op) {
    MILO_ASSERT(i == prop->Size() && op <= kPropInsert, 0x2C);
    if (op == kPropGet)
        node = iref;
    else
        iref = node.Int();
    return true;
}

inline bool PropSync(short &s, DataNode &node, DataArray *prop, int i, PropOp op) {
    MILO_ASSERT(i == prop->Size() && op <= kPropInsert, 0x36);
    if (op == kPropGet)
        node = s;
    else
        s = node.Int();
    return true;
}

inline bool PropSync(bool &b, DataNode &node, DataArray *prop, int i, PropOp op) {
    MILO_ASSERT(i == prop->Size() && op <= kPropInsert, 0x40);
    if (op == kPropGet)
        node = b;
    else
        b = node.Int() != 0;
    return true;
}

inline bool PropSync(Symbol &sym, DataNode &node, DataArray *prop, int i, PropOp op) {
    MILO_ASSERT(i == prop->Size() && op <= kPropInsert, 0x4A);
    if (op == kPropGet)
        node = sym;
    else
        sym = node.Str();
    return true;
}

template <class T>
bool PropSync(Key<T> &key, DataNode &node, DataArray *prop, int i, PropOp op) {
    if (op == kPropUnknown0x40)
        return false;
    else if (i == prop->Size()) {
        return true;
    } else {
        Symbol sym = prop->Sym(i++);
        {
            static Symbol frame("frame");
            if (sym == frame)
                return PropSync(key.frame, node, prop, i, op);
        }
        {
            static Symbol value("value");
            if (sym == value)
                return PropSync(key.value, node, prop, i, op);
        }
    }
    return false;
}

template <class T>
bool PropSync(Keys<T, T> &keys, DataNode &node, DataArray *prop, int i, PropOp op) {
    if (op == kPropUnknown0x40)
        return false;
    else if (i == prop->Size()) {
        MILO_ASSERT(op == kPropSize || op == kPropInsert, 0x10A);
        node = keys.NumKeys();
        return true;
    } else {
        typename Keys<T, T>::iterator it = keys.begin() + prop->Int(i++);
        if (i < prop->Size() || op & (kPropGet | kPropSet | kPropSize)) {
            bool ret = PropSync(*it, node, prop, i, op);
            if (op & kPropSet) {
                std::sort(keys.begin(), keys.end());
            }
            return ret;
        } else if (op == kPropRemove) {
            keys.erase(it);
            return true;
        } else if (op == kPropInsert) {
            Key<T> key;
            if (PropSync(key, node, prop, i, kPropInsert)) {
                keys.insert(it, key);
                std::sort(keys.begin(), keys.end());
                return true;
            }
        }
        return false;
    }
}

template <class T>
bool PropSync(ObjPtr<T> &, DataNode &, DataArray *, int, PropOp);

template <class T>
bool PropSync(ObjOwnerPtr<T> &, DataNode &, DataArray *, int, PropOp);

template <class T>
bool PropSync(ObjPtrList<T, ObjectDir> &, DataNode &, DataArray *, int, PropOp);

template <class T>
bool PropSync(ObjPtrVec<T, ObjectDir> &, DataNode &, DataArray *, int, PropOp);

template <class T>
bool PropSync(ObjList<T> &objList, DataNode &node, DataArray *prop, int i, PropOp op) {
    if (op == kPropUnknown0x40)
        return false;
    else if (i == prop->Size()) {
        MILO_ASSERT(op == kPropSize, 0x1A6);
        node = objList.size();
        return true;
    } else {
        int count = prop->Int(i++);
        typename std::list<T>::iterator it = NextItr(objList.begin(), count);
        if (i < prop->Size() || op & (kPropGet | kPropSet | kPropSize)) {
            return PropSync(*it, node, prop, i, op);
        } else if (op == kPropRemove) {
            objList.erase(it);
            return true;
        } else if (op == kPropInsert) {
            T item(objList.Owner());
            if (PropSync(item, node, prop, i, kPropInsert)) {
                objList.insert(it, item);
                return true;
            }
        }
        return false;
    }
}
