#pragma once
#include "os/Platform.h"
#include "os/Timer.h"
#include "utl/FilePath.h"
#include "utl/MemMgr.h"
#include <list>

enum LoaderPos {
    kLoadFront = 0,
    kLoadBack = 1,
    kLoadFrontStayBack = 2,
    kLoadStayBack = 3,
};

class Loader {
public:
    class Callback {
    public:
        Callback() {}
        virtual ~Callback() {}
        virtual void FinishLoading(Loader *) {}
        virtual void FailedLoading(Loader *) {}
    };
    Loader(const FilePath &, LoaderPos);
    virtual ~Loader();
    virtual const char *DebugText() = 0;
    virtual bool IsLoaded() const = 0;
    virtual const char *StateName() const { return "Unknown"; }

    NEW_OVERLOAD("Loader", 0xA8);
    DELETE_OVERLOAD("Loader", 0xA8);

protected:
    virtual void PollLoading() = 0;

    int unk4; // 0x4
    LoaderPos mPos; // 0x8
    FilePath mFile; // 0xc
    int unk14; // 0x14
    int mHeap; // 0x18
};

typedef Loader *LoaderFactoryFunc(const FilePath &, LoaderPos);

class LoadMgr {
private:
    std::list<Loader *> mLoaders; // 0x0
    unsigned int mPlatform; // 0x8
    bool mEditMode; // 0xC
    bool mCacheMode; // 0xD
    std::list<std::pair<class String, LoaderFactoryFunc *> > mFactories; // 0x10
    float mPeriod; // 0x18
    float unk1c;
    std::list<Loader *> mLoading; // 0x20
    Timer mTimer; // 0x28
    int mAsyncUnload; // 0x58
    LoaderPos mLoaderPos; // 0x5c
public:
    LoadMgr();
    ~LoadMgr();

    void StartAsyncUnload() { mAsyncUnload++; }
    void FinishAsyncUnload() { mAsyncUnload--; }
    bool EditMode() const { return mEditMode; }
    Platform GetPlatform() const { return (Platform)mPlatform; }
    int AsyncUnload() const; // { return mAsyncUnload; }

    void SetEditMode(bool);
    void RegisterFactory(const char *, LoaderFactoryFunc *);
    void PollUntilLoaded(Loader *, Loader *);
};

extern LoadMgr TheLoadMgr;
