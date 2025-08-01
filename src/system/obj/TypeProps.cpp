#include "Data.h"
#include "ObjRef.h"
#include "obj/Object.h"

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

void GetSaveFlags(DataArray* arr, bool& proxy, bool& none){
    static Symbol proxy_save = "proxy_save";
    static Symbol no_save = "no_save";
    if(arr){
        for(int i = 2; i < arr->Size(); i++){
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

TypeProps& TypeProps::operator=(const TypeProps& t){
    ClearAll();
    if(t.mMap){
        mMap = t.mMap->Clone(true, false, 0);
    }
    AddRefObjects();
    return *this;
}
