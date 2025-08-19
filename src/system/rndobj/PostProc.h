#pragma once
#include "obj/Object.h"
#include "rndobj/DOFProc.h"

enum ProcessCmd {
    kProcessNone = 0,
    kProcessWorld = 1,
    kProcessPost = 2,
    kProcessChar = 4,
    kProcessPostChar = 6,
    kProcessAll = 7
};

class ProcCounter {
public:
    ProcCounter();
    void SetProcAndLock(bool);
    void SetEvenOddDisabled(bool);
    ProcessCmd ProcCommands();

private:
    unsigned int SetEmulateFPS(int);

    bool mProcAndLock; // 0x0
    int mCount; // 0x4
    int mSwitch; // 0x8
    int mOdd; // 0xc
    int mFPS; // 0x10
    bool mEvenOddDisabled; // 0x14
    bool mTriFrameRendering; // 0x15
};

class PostProcessor {
public:
    PostProcessor() {}
    virtual ~PostProcessor() {}
    virtual void BeginWorld() {}
    virtual void EndWorld() {}
    virtual void DoPost() {}
    virtual float Priority() { return 1; }
    virtual const char *GetProcType() = 0;
};

class RndPostProc : public Hmx::Object, public PostProcessor {
public:
    virtual ~RndPostProc();
    OBJ_CLASSNAME(PostProc);
    OBJ_SET_TYPE(PostProc);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

    virtual void Select();
    virtual void Unselect();
    virtual void SetPriority(float f) { mPriority = f; }
    virtual void QueueMotionBlurObject(class RndDrawable *) {}
    virtual void SetBloomColor() {}
    virtual void OnSelect();
    virtual void OnUnselect();
    virtual void DoPost();
    virtual float Priority(); // float at 0x4?
    virtual const char *GetProcType() { return "RndPostProc"; }

    static RndPostProc *Current();
    static void ResetDofProc();
    static void Init();
    static void Reset();

protected:
    RndPostProc();

    static RndPostProc *sCurrent;
    static float sBloomLocFactor;
    static DOFOverrideParams sDOFOverride;

    float mPriority; // 0x30
};
