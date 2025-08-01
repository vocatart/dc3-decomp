#include "obj/Object.h"
#include "Dir.h"
#include "Msg.h"
#include "ObjMacros.h"
#include "ObjRef.h"
#include "Object.h"
#include "obj/Data.h"
#include "obj/Utl.h"
#include "os/Debug.h"
#include "os/Platform.h"
#include "os/System.h"
#include "utl/BinStream.h"
#include "utl/Symbol.h"

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

void Hmx::Object::RemoveFromDir() {
    if (mDir && mDir != sDeleting) {
        mDir->RemovingObject(this);
        ObjectDir::Entry *entry = mDir->FindEntry(mName, false);
        if(!entry || entry->obj != this){
            MILO_FAIL("No entry for %s in %s", PathName(this), PathName(mDir));
        }
        entry->obj = nullptr;
    }
}

bool Hmx::Object::HasTypeProps() const {
    if(mTypeProps){
        DataArray* tpMap = mTypeProps->Map();
        return tpMap && tpMap->Size() != 0;
    }
    return false;
}

MsgSinks* Hmx::Object::GetOrAddSinks(){
    if(!mSinks){
        mSinks = new MsgSinks(this);
    }
    return mSinks;
}

void Hmx::Object::AddSink(Hmx::Object* o, Symbol s1, Symbol s2, SinkMode sm, bool b){
    MsgSinks* sinks = GetOrAddSinks();
    sinks->AddSink(o, s1, s2, sm, b);
}

void Hmx::Object::AddPropertySink(Hmx::Object* o, DataArray* a, Symbol s){
    MsgSinks* sinks = GetOrAddSinks();
    sinks->AddPropertySink(o, a, s);
}

void Hmx::Object::MergeSinks(Hmx::Object* o){
    if(o && o->mSinks){
        MsgSinks* sinks = GetOrAddSinks();
        sinks->MergeSinks(o);
    }
}

void Hmx::Object::SaveType(BinStream &bs) {
    bs << 2;
    bs << Type();
}

void Hmx::Object::SaveRest(BinStream &bs) {
    if (!mTypeProps)
        bs << (DataArray *)nullptr;
    else
        mTypeProps->Save(bs);

    if (mNote.empty() || (unsigned char)bs.GetPlatform() != kPlatformNone)
        bs << 0;
    else
        bs << mNote;
}

DataArray* Hmx::Object::ObjectDef(Symbol s){
    Symbol sref;
    if(s == gNullStr){
        sref = "objects";
        s = ClassName();
    }
    else 
        sref = "objects";
    return SystemConfig(sref, s);
}

Hmx::Object::~Object() {}

void Hmx::Object::SetName(const char* name, ObjectDir* dir){
    RemoveFromDir();
    if(!name || *name == '\0'){
        mName = gNullStr;
        mDir = nullptr;
    }
    else {
        MILO_ASSERT(dir, 0xE7);
        mDir = dir;
        ObjectDir::Entry* entry = dir->FindEntry(name, true);
        if(entry->obj){
            MILO_FAIL("%s already exists", name);
        }
        entry->obj = this;
        mName = entry->name;
        dir->AddedObject(this);
    }
}

void Hmx::Object::SetTypeDef(DataArray* def){
    if(mTypeDef != def){
        if(mTypeDef){
            mTypeDef->Release();
            mTypeDef = nullptr;
        }
        ClearAllTypeProps();
        mTypeDef = def;
        if(mTypeDef){
            mTypeDef->AddRef();
        }
    }
}

void Hmx::Object::Save(BinStream &bs) {
    SaveType(bs);
    SaveRest(bs);
}

DataNode Hmx::Object::OnIterateRefs(const DataArray *da) {
    DataNode *var = da->Var(2);
    DataNode node(*var);
    for (ObjRef *it = mRefs.next; it != &mRefs; it = mRefs.next) {
        *var = (*it++).RefOwner();
        for (int i = 3; i < da->Size(); i++) {
            da->Command(i)->Execute(true);
        }
    }
    *var = node;
    return 0;
}

void Hmx::Object::BroadcastPropertyChange(DataArray* a){
    Symbol s;
    if(mSinks){
        s = mSinks->GetPropSyncHandler(a);
    }
    ExportPropertyChange(a, s);
}

int Hmx::Object::PropertySize(DataArray *prop) {
    static DataNode n;
    if (SyncProperty(n, prop, 0, kPropSize)) {
        return n.Int();
    } else {
        MILO_ASSERT(prop->Size() == 1, 0x208);
        Symbol name = prop->Sym(0);
        const DataNode *a = mTypeProps->KeyValue(name, false);
        if (a == nullptr) {
            if (mTypeDef != nullptr) {
                a = &mTypeDef->FindArray(name)->Evaluate(1);
            } else
                MILO_FAIL("%s: property %s not found", PathName(this), name);
        }
        MILO_ASSERT(a->Type() == kDataArray, 0x21B);
        return a->UncheckedArray()->Size();
    }
    return 0;
}

void Hmx::Object::RemoveProperty(DataArray *prop) {
    static DataNode n;
    if (!SyncProperty(n, prop, 0, kPropRemove)) {
        MILO_ASSERT(prop->Size() == 2, 0x235);
        if(mTypeProps){
            mTypeProps->RemoveArrayValue(prop->Sym(0), prop->Int(1));
        }
    }
}

void Hmx::Object::Export(DataArray* a, bool b){
    if(b) HandleType(a);
    if(mSinks) mSinks->Export(a);
}

BEGIN_PROPSYNCS(Hmx::Object)
    SYNC_PROP_SET(name, mName, SetName(_val.Str(), mDir))
    SYNC_PROP_SET(type, Type(), SetType(_val.Sym()))
END_PROPSYNCS

BEGIN_HANDLERS(Hmx::Object)
    HANDLE(get, OnGet)
    HANDLE_EXPR(get_array, PropertyArray(_msg->Sym(2)))
    HANDLE_EXPR(size, PropertySize(_msg->Array(2)))
    HANDLE(set, OnSet)
    HANDLE_ACTION(insert, InsertProperty(_msg->Array(2), _msg->Evaluate(3)))
    HANDLE_ACTION(remove, RemoveProperty(_msg->Array(2)))
    HANDLE_ACTION(clear, PropertyClear(_msg->Array(2)))
    HANDLE(append, OnPropertyAppend)
    HANDLE_EXPR(has, Property(_msg->Array(2), false) != nullptr)
    HANDLE_EXPR(prop_handle, HandleProperty(_msg->Array(2), _msg, true))
    HANDLE_ACTION(copy, Copy(_msg->Obj<Hmx::Object>(2), (CopyType)_msg->Int(3)))
    HANDLE_EXPR(class_name, ClassName())
    HANDLE_EXPR(name, mName)
    HANDLE_EXPR(note, mNote)
    HANDLE_ACTION(set_note, SetNote(_msg->Str(2)))
    HANDLE(iterate_refs, OnIterateRefs)
    HANDLE_EXPR(dir, mDir)
    HANDLE_ACTION(set_name, SetName(_msg->Str(2), _msg->Size() > 3 ? _msg->Obj<ObjectDir>(3) : Dir()))
    HANDLE_ACTION(set_type, SetType(_msg->Sym(2)))
    HANDLE_EXPR(is_a, IsASubclass(ClassName(), _msg->Sym(2)))
    HANDLE_EXPR(get_type, Type())
    HANDLE_EXPR(get_heap, AllocHeapName())
    HANDLE(get_types_list, OnGetTypeList)
    HANDLE_ARRAY(mTypeDef)
    HANDLE(add_sink, OnAddSink)
    HANDLE(remove_sink, OnRemoveSink)
END_HANDLERS
