#pragma once
#include "ObjMacros.h"
#include "Object.h"
#include "obj/Object.h"

enum InlineDirType {
    kInlineNever = 0,
    kInlineCached = 1,
    kInlineAlways = 2,
    kInlineCachedShared = 3
};

class ObjectDir : public virtual Hmx::Object {
protected:
    struct Entry {
        const char* name;
        Hmx::Object* obj;
    };

    // class Viewport {
    //     Transform mXfm;
    // };

    // struct InlinedDir {
    //     ObjDirPtr<ObjectDir> dir; // 0x0
    //     FilePath file; // 0x14
    //     bool shared; // 0x1c
    //     InlineDirType inlineDirType; // 0x20
    // };

    ObjectDir();
public:
    enum ViewportId {
        kNumViewports = 7
    };
    
    // Hmx::Object
    virtual ~ObjectDir();
    OBJ_CLASSNAME(ObjectDir);
    virtual void SetType(Symbol);
    virtual DataNode Handle(DataArray*, bool);
    virtual bool SyncProperty(DataNode&, DataArray*, int, PropOp);
    virtual void Save(BinStream&);
    virtual void Copy(const Hmx::Object*, CopyType);
    virtual void Load(BinStream&);
    virtual void PostSave(BinStream&);
    virtual void SetName(const char*, ObjectDir*);
    virtual ObjectDir* DataDir();
    virtual void PreLoad(BinStream&);
    virtual void PostLoad(BinStream&);
    // ObjectDir
    virtual void SetProxyFile(const FilePath&, bool);
    virtual const FilePath& ProxyFile();
    virtual void SetSubDir(bool);
    virtual DataArrayPtr GetExposedProperties();
    virtual void SyncObjects();
    virtual void ResetEditorState();
    virtual InlineDirType InlineSubDirType();

    void SetCurViewport(ViewportId, Hmx::Object*);
};
