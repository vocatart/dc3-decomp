#pragma once
#include "math/Mtx.h"
#include "obj/ObjRef.h"
#include "obj/Object.h"
#include "utl/BinStream.h"
#include "utl/FilePath.h"
#include "utl/KeylessHash.h"
#include "utl/MemMgr.h"
#include "utl/StringTable.h"
#include <vector>

enum InlineDirType {
    kInlineNever = 0,
    kInlineCached = 1,
    kInlineAlways = 2,
    kInlineCachedShared = 3
};

// ObjDirPtr class def goes here
template <class T>
class ObjDirPtr : public ObjRefConcrete<T, ObjectDir> {
public:
    class DirLoader *mLoader; // 0x10
};

class ObjectDir : public virtual Hmx::Object {
    friend class Hmx::Object;

public:
    enum ViewportId {
        kNumViewports = 7
    };

protected:
    struct Entry {
        const char *name;
        Hmx::Object *obj;
    };

    class Viewport {
        Transform mXfm;
    };

    struct InlinedDir {
        ObjDirPtr<ObjectDir> dir; // 0x0
        FilePath file; // 0x14
        bool shared; // 0x1c
        InlineDirType inlineDirType; // 0x20
    };

    KeylessHash<const char *, Entry> mHashTable; // 0x8
    StringTable mStringTable; // 0x28
    FilePath mProxyFile; // 0x3c
    bool mProxyOverride; // 0x44
    InlineDirType mInlineProxyType; // 0x48
    DirLoader *mLoader; // 0x4c
    std::vector<ObjDirPtr<ObjectDir> > mSubDirs; // 0x50
    bool mIsSubDir; // 0x5c
    InlineDirType mInlineSubDirType; // 0x60
    const char *mPathName; // 0x64
    FilePath mStoredFile; // 0x68
    std::vector<InlinedDir> mInlinedDirs; // 0x70
    std::vector<Viewport> mViewports; // 0x7c
    ViewportId mCurViewportID; // 0x88
    Hmx::Object *unk8c; // 0x8c
    Hmx::Object *mCurCam; // 0x90
    int mAlwaysInlined; // 0x94
    const char *mAlwaysInlineHash; // 0x98

    ObjectDir();
    Entry *FindEntry(const char *, bool);

public:
    // Hmx::Object
    virtual ~ObjectDir();
    OBJ_CLASSNAME(ObjectDir);
    OBJ_SET_TYPE(ObjectDir);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void PostSave(BinStream &);
    virtual void SetName(const char *, ObjectDir *);
    virtual ObjectDir *DataDir();
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // ObjectDir
    virtual void SetProxyFile(const FilePath &, bool);
    virtual const FilePath &ProxyFile();
    virtual void SetSubDir(bool);
    virtual DataArrayPtr GetExposedProperties();
    virtual void SyncObjects();
    virtual void ResetEditorState();
    virtual InlineDirType InlineSubDirType();

    void SetCurViewport(ViewportId, Hmx::Object *);

    NEW_OVERLOAD(StaticClassName().Str(), 0x111);
    DELETE_OVERLOAD(StaticClassName().Str(), 0x111);

protected:
    virtual void AddedObject(Hmx::Object *) {}
    virtual void RemovingObject(Hmx::Object *);
    virtual void OldLoadProxies(BinStream &, int);
};
