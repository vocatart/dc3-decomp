#pragma once
#include "rndobj/Trans.h"

class RndTransProxy : public RndTransformable {
public:
    // Hmx::Object
    virtual ~RndTransProxy() {}
    OBJ_CLASSNAME(TransProxy);
    OBJ_SET_TYPE(TransProxy);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void PreSave(BinStream &);
    virtual void PostSave(BinStream &);

protected:
    RndTransProxy();

    /** "Proxy object this will look into." */
    ObjPtr<ObjectDir> mProxy; // 0xc0
    /** "The part inside it" */
    Symbol mPart; // 0xd4
};
