#pragma once
#include "obj/Data.h"
#include "obj/Dir.h"
#include "obj/Object.h"
#include "utl/FilePath.h"
#include "utl/Symbol.h"

class ResourceDirBase {
public:
    static bool MakeResourcePath(FilePath &, Symbol, Symbol, const char *);
    static DataNode GetFileList(Symbol, Symbol);

protected:
    static const char *GetResourcesPath(Symbol, Symbol);
};

template <class T>
class ResourceDirPtr : public ObjDirPtr<T>, public ResourceDirBase {
public:
    // i have no idea if this is right
    ResourceDirPtr(Hmx::Object *owner) : unk14(owner) {}

    void SetName(const char *, bool);
    const char *GetName() const;

    Hmx::Object *unk14; // 0x14
};
