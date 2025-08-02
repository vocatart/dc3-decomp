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

std::map<Symbol, ObjectFunc *> Hmx::Object::sFactories;
DataArrayPtr gPropPaths[8] = {
    DataArrayPtr(new DataArray(1)), DataArrayPtr(new DataArray(1)),
    DataArrayPtr(new DataArray(1)), DataArrayPtr(new DataArray(1)),
    DataArrayPtr(new DataArray(1)), DataArrayPtr(new DataArray(1)),
    DataArrayPtr(new DataArray(1)), DataArrayPtr(new DataArray(1))
};
MsgSinks gSinks(nullptr);

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

ObjectDir *Hmx::Object::DataDir() { return mDir ? mDir : ObjectDir::sMainDir; }

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
        if (!entry || entry->obj != this) {
            MILO_FAIL("No entry for %s in %s", PathName(this), PathName(mDir));
        }
        entry->obj = nullptr;
    }
}

DataArray *GetNextPropPath() {
    for (int i = 0; i < 8; i++) {
        if (gPropPaths[i]->RefCount() == 1) {
            return gPropPaths[i];
        }
    }
    MILO_FAIL("Recursive SetProperty call count greater than %d!", 8);
    return nullptr;
}

bool Hmx::Object::HasTypeProps() const {
    if (mTypeProps) {
        DataArray *tpMap = mTypeProps->Map();
        return tpMap && tpMap->Size() != 0;
    }
    return false;
}

MsgSinks *Hmx::Object::GetOrAddSinks() {
    if (!mSinks) {
        mSinks = new MsgSinks(this);
    }
    return mSinks;
}

void Hmx::Object::AddSink(Hmx::Object *o, Symbol s1, Symbol s2, SinkMode sm, bool b) {
    MsgSinks *sinks = GetOrAddSinks();
    sinks->AddSink(o, s1, s2, sm, b);
}

void Hmx::Object::AddPropertySink(Hmx::Object *o, DataArray *a, Symbol s) {
    MsgSinks *sinks = GetOrAddSinks();
    sinks->AddPropertySink(o, a, s);
}

void Hmx::Object::MergeSinks(Hmx::Object *o) {
    if (o && o->mSinks) {
        MsgSinks *sinks = GetOrAddSinks();
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

DataArray *Hmx::Object::ObjectDef(Symbol s) {
    Symbol sref;
    if (s == gNullStr) {
        sref = "objects";
        s = ClassName();
    } else
        sref = "objects";
    return SystemConfig(sref, s);
}

void Hmx::Object::LoadType(BinStream &bs) {
    int revs;
    bs >> revs;
    BinStreamRev bsrev(bs, revs);
    // how does binstreamrev work?
}

Hmx::Object::~Object() {}

void Hmx::Object::SetName(const char *name, ObjectDir *dir) {
    RemoveFromDir();
    if (!name || *name == '\0') {
        mName = gNullStr;
        mDir = nullptr;
    } else {
        MILO_ASSERT(dir, 0xE7);
        mDir = dir;
        ObjectDir::Entry *entry = dir->FindEntry(name, true);
        if (entry->obj) {
            MILO_FAIL("%s already exists", name);
        }
        entry->obj = this;
        mName = entry->name;
        dir->AddedObject(this);
    }
}

void Hmx::Object::SetTypeDef(DataArray *def) {
    if (mTypeDef != def) {
        if (mTypeDef) {
            mTypeDef->Release();
            mTypeDef = nullptr;
        }
        ClearAllTypeProps();
        mTypeDef = def;
        if (mTypeDef) {
            mTypeDef->AddRef();
        }
    }
}

void Hmx::Object::ChainSource(Hmx::Object *source, Hmx::Object *o2) {
    MILO_ASSERT(source, 0x29D);
    if (!o2)
        o2 = this;
    if (mSinks) {
        source->GetOrAddSinks()->AddSink(this, gNullStr);
    }
}

void Hmx::Object::Save(BinStream &bs) {
    SaveType(bs);
    SaveRest(bs);
}

void Hmx::Object::InitObject() {
    static DataArray *objects = SystemConfig("objects");
    static Symbol init = "init";
    DataArray *def = ObjectDef(gNullStr)->FindArray(init, false);
    if (def) {
        def->ExecuteScript(1, this, nullptr, 1);
    }
}

DataNode Hmx::Object::HandleType(DataArray *msg) {
    Symbol t = msg->Sym(1);
    DataArray *handler = nullptr;
    if (mTypeDef) {
        handler = mTypeDef->FindArray(t, false);
    }
    if (handler) {
        MessageTimer timer(this, t);
        return handler->ExecuteScript(1, this, (const DataArray *)msg, 2);
    } else
        return DataNode(kDataUnhandled, 0);
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

void Hmx::Object::ExportPropertyChange(DataArray *a, Symbol s) {
    if (!s.Null()) {
        MILO_ASSERT(mSinks, 0x17F);
        static Message msg("blah", 0);
        msg.SetType(s);
        msg[0] = DataNode(a, kDataArray);
        Export(msg, true);
    }
}

void Hmx::Object::BroadcastPropertyChange(DataArray *a) {
    Symbol s;
    if (mSinks) {
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
        if (mTypeProps) {
            mTypeProps->RemoveArrayValue(prop->Sym(0), prop->Int(1));
        }
    }
}

void Hmx::Object::Export(DataArray *a, bool b) {
    if (b)
        HandleType(a);
    if (mSinks)
        mSinks->Export(a);
}

BEGIN_PROPSYNCS(Hmx::Object)
    SYNC_PROP_SET(name, mName, SetName(_val.Str(), mDir))
    SYNC_PROP_SET(type, Type(), SetType(_val.Sym()))
    SYNC_PROP(sinks, mSinks ? *mSinks : gSinks)
END_PROPSYNCS

void Hmx::Object::Copy(const Hmx::Object *o, CopyType ty) {
    if (ty != kCopyFromMax) {
        mNote = o->mNote;
        if (ClassName() == o->ClassName()) {
            SetTypeDef(o->mTypeDef);
            if (o->HasTypeProps() && !mTypeProps) {
                mTypeProps = new TypeProps(this);
            } else if (!o->HasTypeProps()) {
                RELEASE(mTypeProps);
            }
            if (mTypeProps) {
                *mTypeProps = *o->mTypeProps;
            }
        } else if (o->mTypeDef || mTypeDef) {
            MILO_WARN(
                "Can't copy type \"%s\" or type props of %s to %s, different classes %s and %s",
                o->Type(),
                mName,
                o->mName,
                o->ClassName(),
                ClassName()
            );
        }
    }
}

void Hmx::Object::Load(BinStream &bs) {
    LoadType(bs);
    LoadRest(bs);
}

DataNode Hmx::Object::OnSet(const DataArray *a) {
    MILO_ASSERT_FMT(
        a->Size() % 2 == 0,
        "Uneven number of properties (file %s, line %d)",
        a->File(),
        a->Line()
    );
    for (int i = 2; i < a->Size(); i += 2) {
        const DataNode &n = a->Evaluate(i);
        if (n.Type() == kDataSymbol) {
            const char *str = n.UncheckedStr();
            SetProperty(STR_TO_SYM(str), a->Evaluate(i + 1));
        } else {
            if (n.Type() != kDataArray) {
                String str;
                n.Print(str, true, 0);
                MILO_FAIL(
                    "Data %s is not array or symbol (file %s, line %d)",
                    str.c_str(),
                    a->File(),
                    a->Line()
                );
            }
            SetProperty(n.UncheckedArray(), a->Evaluate(i + 1));
        }
    }
    return 0;
}

DataNode Hmx::Object::OnPropertyAppend(const DataArray *da) {
    DataArray *arr = da->Array(2);
    int size = PropertySize(arr);
    DataArray *cloned = arr->Clone(true, false, 1);
    cloned->Node(cloned->Size() - 1) = size;
    InsertProperty(cloned, da->Evaluate(3));
    cloned->Release();
    return size;
}

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
    HANDLE_ACTION(
        set_name,
        SetName(_msg->Str(2), _msg->Size() > 3 ? _msg->Obj<ObjectDir>(3) : Dir())
    )
    HANDLE_ACTION(set_type, SetType(_msg->Sym(2)))
    HANDLE_EXPR(is_a, IsASubclass(ClassName(), _msg->Sym(2)))
    HANDLE_EXPR(get_type, Type())
    HANDLE_EXPR(get_heap, AllocHeapName())
    HANDLE(get_types_list, OnGetTypeList)
    HANDLE_ARRAY(mTypeDef)
    HANDLE(add_sink, OnAddSink)
    HANDLE(remove_sink, OnRemoveSink)
END_HANDLERS
