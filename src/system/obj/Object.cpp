#include "obj/Object.h"
#include "ObjMacros.h"
#include "ObjRef.h"
#include "Object.h"
#include "obj/Data.h"
#include "os/System.h"
#include "utl/BinStream.h"
#include "utl/Symbol.h"

void Hmx::Object::SetNote(const char *note) { mNote = note; }

bool Hmx::Object::Replace(ObjRef *ref, Hmx::Object *obj) {
    if (mSinks)
        return mSinks->Replace(ref, obj);
    else
        return false;
}

void Hmx::Object::RemovePropertySink(Hmx::Object *o, DataArray *a) {
    if (mSinks)
        mSinks->RemovePropertySink(o, a);
}

bool Hmx::Object::HasPropertySink(Hmx::Object *o, DataArray *a) {
    if (mSinks)
        return mSinks->HasPropertySink(o, a);
    else
        return false;
}

void Hmx::Object::RemoveSink(Hmx::Object *o, Symbol s) {
    if (mSinks)
        mSinks->RemoveSink(o, s);
}

Hmx::Object::Object()
    : mTypeProps(nullptr), mTypeDef(nullptr), mName(gNullStr), mDir(nullptr),
      mSinks(nullptr) {
    mRefs.Init();
}

void Hmx::Object::ReplaceRefs(Hmx::Object *obj) { mRefs.ReplaceList(obj); }

int Hmx::Object::RefCount() const { return mRefs.RefCount(); }

Hmx::Object::~Object() {}

void Hmx::Object::Save(BinStream& bs){
    SaveType(bs);
    SaveRest(bs);
}

DataNode Hmx::Object::OnIterateRefs(const DataArray *da) {
    DataNode *var = da->Var(2);
    DataNode node(*var);
    for(ObjRef* it = mRefs.next; it != &mRefs; it = mRefs.next){
        *var = (*it++).RefOwner();
        for (int i = 3; i < da->Size(); i++) {
            da->Command(i)->Execute(true);
        }
    }
    *var = node;
    return 0;
}

BEGIN_HANDLERS(Hmx::Object)
END_HANDLERS
