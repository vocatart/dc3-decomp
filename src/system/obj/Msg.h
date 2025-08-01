#pragma once
#include "obj/Data.h"
#include "obj/ObjRef.h"
#include "obj/Object.h"

class MsgSinks {
public:
    bool Replace(ObjRef*, Hmx::Object*);
    void RemovePropertySink(Hmx::Object*, DataArray*);
    bool HasPropertySink(Hmx::Object*, DataArray*);
    void RemoveSink(Hmx::Object*, Symbol);
};
