#pragma once
#include "rndobj/Poll.h"
#include "rndobj/PostProc.h"
#include "utl/MemMgr.h"

/** "Settings for terr world effects." */
class RndPostProcMgr : public RndPollable {
public:
    // Hmx::Object
    virtual ~RndPostProcMgr() {}
    OBJ_CLASSNAME(PostProcMgr);
    OBJ_SET_TYPE(PostProcMgr);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    // RndPollable
    virtual void Poll();
    virtual void Enter();
    virtual void Exit();

    OBJ_MEM_OVERLOAD(0x22);

protected:
    RndPostProcMgr();

    /** "Primary postproc to use.  When blending,
        the blend result will be written into this postproc." */
    ObjPtr<RndPostProc> mSelectedPostProc; // 0x8
    RndPostProc *unk1c; // 0x1c
    ObjPtr<RndPostProc> unk20; // 0x20
    float unk34; // 0x34
    float unk38; // 0x38
};
