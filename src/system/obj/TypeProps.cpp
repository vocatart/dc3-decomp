#include "Data.h"
#include "ObjRef.h"
#include "obj/Object.h"
#include "os/Debug.h"
#include "utl/Symbol.h"

DataNode *TypeProps::KeyValue(Symbol key, bool fail) const {
    if (mMap) {
        for (int i = mMap->Size() - 2; i >= 0; i -= 2) {
            if (mMap->Node(i).UncheckedStr() == key.Str()) {
                return &mMap->Node(i + 1);
            }
        }
    }
    if (fail) {
        MILO_FAIL("Key %s not found", key);
    }
    return nullptr;
}

void GetSaveFlags(DataArray *arr, bool &proxy, bool &none) {
    static Symbol proxy_save = "proxy_save";
    static Symbol no_save = "no_save";
    if (arr) {
        for (int i = 2; i < arr->Size(); i++) {
            if (arr->Sym(i) == proxy_save)
                proxy = true;
            else if (arr->Sym(i) == no_save)
                none = true;
            else
                MILO_WARN("Unknown type def attribute %s", arr->Sym(i));
        }
    }
}

int TypeProps::Size() const {
    if (mMap)
        return mMap->Size() / 2;
    else
        return 0;
}

void TypeProps::ClearKeyValue(Symbol key) {
    if (mMap) {
        for (int i = mMap->Size() - 2; i >= 0; i -= 2) {
            const char *cur_str = mMap->UncheckedStr(i);
            if (STR_TO_SYM(cur_str) == key) {
                DataNode &val = mMap->Node(i + 1);
                if (val.Type() == kDataObject) {
                    Hmx::Object *obj = val.UncheckedObj();
                    if (obj) {
                        mObjects.remove(obj);
                    }
                }
                mMap->Remove(i);
                mMap->Remove(i);
                if (mMap->Size() == 0 && mMap) {
                    mMap->Release();
                    mMap = nullptr;
                }
            }
        }
    }
}

bool TypeProps::Replace(ObjRef *ref, Hmx::Object *obj) {
    if (!mMap)
        return false;
    Hmx::Object *refObj = ref->GetObj();
    for (int i = mMap->Size() - 1; i > 0; i -= 2) {
        DataNode &node = mMap->Node(i);
        if (node.Type() == kDataObject) {
            ReplaceObject(node, refObj, obj);
        } else if (node.Type() == kDataArray) {
            DataArray *inner = node.UncheckedArray();
            for (int j = inner->Size() - 1; j >= 0; j--) {
                DataNode &node2 = inner->Node(j);
                if (node2.Type() == kDataObject) {
                    ReplaceObject(node2, refObj, obj);
                }
            }
        }
    }
    return true;
}

void TypeProps::ReleaseObjects() {
    if (mMap)
        mObjects.clear();
}

void TypeProps::AddRefObjects() {
    if (mMap) {
        for (int i = mMap->Size() - 1; i > 0; i -= 2) {
            DataNode &node = mMap->Node(i);
            if (node.Type() == kDataObject) {
                Hmx::Object *obj = node.UncheckedObj();
                if (obj)
                    mObjects.push_back(obj);
            } else if (node.Type() == kDataArray) {
                DataArray *inner = node.UncheckedArray();
                for (int j = inner->Size() - 1; j >= 0; j--) {
                    DataNode &node2 = inner->Node(j);
                    if (node2.Type() == kDataObject) {
                        Hmx::Object *obj = node.UncheckedObj();
                        if (obj)
                            mObjects.push_back(obj);
                    }
                }
            }
        }
    }
}

void TypeProps::ClearAll() {
    ReleaseObjects();
    if (mMap) {
        mMap->Release();
        mMap = nullptr;
    }
}

DataArray *TypeProps::GetArray(Symbol key) {
    DataArray *typeDef = mOwner->TypeDef();
    DataNode *n = KeyValue(key, false);
    DataArray *ret;
    if (!n) {
        MILO_ASSERT(typeDef, 0x18);
        DataArray *keyArray = typeDef->FindArray(key);
        DataArray *cloned = keyArray->Array(1)->Clone(true, false, 0);
        SetKeyValue(key, DataNode(cloned, kDataArray), true);
        ret = cloned;
        cloned->Release();
    } else {
        MILO_ASSERT(n->Type() == kDataArray, 0x1F);
        ret = n->UncheckedArray();
    }
    return ret;
}

void TypeProps::SetArrayValue(Symbol key, int i, const DataNode &value) {
    DataNode &n = GetArray(key)->Node(i);
    if (n.Type() == kDataObject) {
        Hmx::Object *obj = n.UncheckedObj();
        if (obj) {
            mObjects.remove(obj);
        }
    }
    n = value;
    if (n.Type() == kDataObject) {
        Hmx::Object *obj = n.UncheckedObj();
        if (obj) {
            mObjects.push_back(obj);
        }
    }
}

void TypeProps::RemoveArrayValue(Symbol key, int i) {
    DataArray *a = GetArray(key);
    DataNode &n = a->Node(i);
    if (n.Type() == kDataObject) {
        Hmx::Object *obj = n.UncheckedObj();
        if (obj) {
            mObjects.remove(obj);
        }
    }
    a->Remove(i);
}

void TypeProps::InsertArrayValue(Symbol key, int i, const DataNode &value) {
    GetArray(key)->Insert(i, value);
    if (value.Type() == kDataObject) {
        Hmx::Object *obj = value.UncheckedObj();
        if (obj) {
            mObjects.push_back(obj);
        }
    }
}

TypeProps &TypeProps::operator=(const TypeProps &t) {
    ClearAll();
    if (t.mMap) {
        mMap = t.mMap->Clone(true, false, 0);
    }
    AddRefObjects();
    return *this;
}
