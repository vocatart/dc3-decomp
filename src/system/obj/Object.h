#pragma once
#include "ObjMacros.h"
#include "obj/ObjMacros.h"
#include "obj/ObjRef.h"
#include "obj/Data.h"
#include "obj/TypeProps.h"
#include "obj/Msg.h"
#include "obj/PropSync_p.h"
#include "utl/BinStream.h"
#include "utl/Symbol.h"

namespace Hmx {

    class Object : public ObjRefOwner {
    private:
        DataNode OnIterateRefs(const DataArray*);
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

        Object();
        virtual ~Object();
        virtual Object *RefOwner() { return this; }
        virtual bool Replace(ObjRef *, Hmx::Object *);
        OBJ_CLASSNAME(Object);
        virtual void SetType(Symbol classname);
        // OBJ_SET_TYPE(Object);
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

        void SetNote(const char *);
        void ReplaceRefs(Hmx::Object *);
        int RefCount() const;
        void RemovePropertySink(Hmx::Object*, DataArray*);
        bool HasPropertySink(Hmx::Object*, DataArray*);
        void RemoveSink(Hmx::Object*, Symbol);
        void SaveType(BinStream&);
        void SaveRest(BinStream&);
    };

}
