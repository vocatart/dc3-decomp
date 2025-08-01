#pragma once
#include "Object.h"
#include "obj/ObjPtr_p.h"

struct ObjVersion {
    ObjVersion(int i, Hmx::Object *o) : revs(i), obj(nullptr, o) {}
    ~ObjVersion() {}

    ObjPtr<Hmx::Object> obj;
    int revs;
};
