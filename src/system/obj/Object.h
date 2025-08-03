#pragma once
#include "obj/ObjMacros.h"
#include "obj/ObjPtr_p.h"
#include "obj/ObjRef.h"
#include "obj/Data.h"
#include "obj/PropSync_p.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"
#include "utl/Symbol.h"
#include <map>

class MsgSinks;
namespace Hmx {
    class Object;
}

class TypeProps : public ObjRefOwner {
private:
    DataArray *mMap; // 0x4
    Hmx::Object *mOwner; // 0x8
    ObjPtrList<Hmx::Object> mObjects; // 0xc

    void ReplaceObject(DataNode &, Hmx::Object *, Hmx::Object *);
    void ReleaseObjects();
    void AddRefObjects();
    void ClearAll();

public:
    TypeProps(Hmx::Object *o)
        : mOwner(o), mMap(nullptr), mObjects(this, kObjListOwnerControl) {}
    virtual ~TypeProps() { ClearAll(); }
    virtual Hmx::Object *RefOwner() const { return mOwner; }
    virtual bool Replace(ObjRef *, Hmx::Object *);

    DataNode *KeyValue(Symbol, bool) const;
    int Size() const;
    void ClearKeyValue(Symbol);
    void SetKeyValue(Symbol, const DataNode &, bool);
    DataArray *GetArray(Symbol);
    void SetArrayValue(Symbol, int, const DataNode &);
    void RemoveArrayValue(Symbol, int);
    void InsertArrayValue(Symbol, int, const DataNode &);
    void Load(BinStreamRev &);
    TypeProps &operator=(const TypeProps &);
    void Save(BinStream &);
    DataArray *Map() const { return mMap; }

    NEW_OVERLOAD("TypeProps", 0x485);
    DELETE_OVERLOAD("TypeProps", 0x485);
};

typedef Hmx::Object *ObjectFunc(void);

namespace Hmx {

    class Object : public ObjRefOwner {
    private:
        void RemoveFromDir();

        DataNode OnIterateRefs(const DataArray *);
        DataNode OnSet(const DataArray *);
        DataNode OnPropertyAppend(const DataArray *);
        DataNode OnGetTypeList(const DataArray *);
        DataNode OnAddSink(DataArray *);
        DataNode OnRemoveSink(DataArray *);
        void ExportPropertyChange(DataArray *, Symbol);

        static std::map<Symbol, ObjectFunc *> sFactories;

    protected:
        static Object *sDeleting;

        MsgSinks *GetOrAddSinks();
        DataNode OnGet(const DataArray *);
        void BroadcastPropertyChange(DataArray *);
        void BroadcastPropertyChange(Symbol);

    public:
        ObjRef mRefs; // 0x4
        TypeProps *mTypeProps; // 0x10
        DataArray *mTypeDef; // 0x14
        String mNote; // 0x18
        const char *mName; // 0x20
        ObjectDir *mDir; // 0x24
        MsgSinks *mSinks; // 0x28

        enum CopyType {
            kCopyDeep = 0,
            kCopyShallow = 1,
            kCopyFromMax = 2
        };

        enum SinkMode {
            /** "does a Handle to the sink, this gets all c handlers, type handling, and
             * exporting." */
            kHandle = 0,
            /** "just Exports to the sink, so no c or type handling" */
            kExport = 1,
            /** "just calls HandleType, good if know that particular thing is only ever
             * type handled." */
            kType = 2,
            /** "do type handling and exporting using Export, no C handling" */
            kExportType = 3,
        };

        Object();
        virtual ~Object();
        virtual Object *RefOwner() const { return const_cast<Object *>(this); }
        virtual bool Replace(ObjRef *, Hmx::Object *);
        OBJ_CLASSNAME(Object);
        OBJ_SET_TYPE(Object);
        virtual DataNode Handle(DataArray *, bool);
        virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
        virtual void InitObject();
        virtual void Save(BinStream &);
        virtual void Copy(const Hmx::Object *, CopyType);
        virtual void Load(BinStream &);
        virtual void PreSave(BinStream &) {}
        virtual void PostSave(BinStream &) {}
        virtual void Print() {}
        virtual void Export(DataArray *, bool);
        virtual void SetTypeDef(DataArray *);
        virtual DataArray *ObjectDef(Symbol);
        virtual void SetName(const char *, ObjectDir *);
        virtual ObjectDir *DataDir();
        virtual void PreLoad(BinStream &);
        virtual void PostLoad(BinStream &) {}
        virtual const char *FindPathName();

        Symbol Type() const {
            if (mTypeDef)
                return mTypeDef->Sym(0);
            else
                return Symbol();
        }
        void SetNote(const char *note) { mNote = note; }
        ObjectDir *Dir() const { return mDir; }
        const char *Name() const { return mName; }
        const char *AllocHeapName() { return MemHeapName(MemFindAddrHeap(this)); }

        void ReplaceRefs(Hmx::Object *);
        int RefCount() const;
        void RemovePropertySink(Hmx::Object *, DataArray *);
        bool HasPropertySink(Hmx::Object *, DataArray *);
        void RemoveSink(Hmx::Object *, Symbol);
        void SaveType(BinStream &);
        void SaveRest(BinStream &);
        void ClearAllTypeProps();
        bool HasTypeProps() const;
        void
        AddSink(Hmx::Object *, Symbol, Symbol = Symbol(), SinkMode = kHandle, bool = true);
        void AddPropertySink(Hmx::Object *, DataArray *, Symbol);
        void MergeSinks(Hmx::Object *);
        DataNode PropertyArray(Symbol);
        int PropertySize(DataArray *);
        void InsertProperty(DataArray *, const DataNode &);
        void RemoveProperty(DataArray *);
        void PropertyClear(DataArray *);
        const DataNode *Property(DataArray *, bool) const;
        const DataNode *Property(Symbol, bool) const;
        DataNode HandleProperty(DataArray *, DataArray *, bool);
        DataNode HandleType(DataArray *);
        void ChainSource(Hmx::Object *, Hmx::Object *);
        void LoadType(BinStream &);
        void LoadRest(BinStream &);
        void SetProperty(Symbol, const DataNode &);
        void SetProperty(DataArray *, const DataNode &);

        static Object *NewObject(Symbol);
        static bool RegisteredFactory(Symbol);
        static void RegisterFactory(Symbol, ObjectFunc *);
    };

}
inline TextStream &operator<<(TextStream &ts, const Hmx::Object *obj) {
    if (obj)
        ts << obj->Name();
    else
        ts << "<null>";
    return ts;
}
