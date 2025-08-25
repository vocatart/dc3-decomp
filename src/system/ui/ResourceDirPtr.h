#pragma once
#include "obj/Data.h"
#include "obj/Dir.h"
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
    void SetName(const char *, bool);
    const char *GetName() const;

    // member at unk14
};
