#pragma once
#include "obj/Data.h"
#include "obj/ObjRef.h"
#include "obj/Object.h"

class MsgSinks {
public:
    MsgSinks(Hmx::Object*);
    bool Replace(ObjRef*, Hmx::Object*);
    void RemovePropertySink(Hmx::Object*, DataArray*);
    bool HasPropertySink(Hmx::Object*, DataArray*);
    void RemoveSink(Hmx::Object*, Symbol);
    void AddSink(Hmx::Object*, Symbol, Symbol = Symbol(), Hmx::Object::SinkMode = Hmx::Object::kHandle, bool = true);
    void AddPropertySink(Hmx::Object*, DataArray*, Symbol);
    void MergeSinks(Hmx::Object*);
    Symbol GetPropSyncHandler(DataArray*);
    void Export(DataArray*);
};
