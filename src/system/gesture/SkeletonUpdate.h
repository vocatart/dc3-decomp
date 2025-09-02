#pragma once
#include "gesture/CameraInput.h"
#include "gesture/Skeleton.h"
#include "gesture/SkeletonHistory.h"
#include "obj/Object.h"
#include "os/CritSec.h"

class SkeletonUpdate;

class SkeletonUpdateHandle {
public:
    SkeletonUpdateHandle(SkeletonUpdate *);
    ~SkeletonUpdateHandle();

    std::vector<SkeletonCallback *> &Callbacks();
    CameraInput *GetCameraInput() const;
    void SetCameraInput(CameraInput *);
    void RemoveCallback(SkeletonCallback *);
    void AddCallback(SkeletonCallback *);
    bool HasCallback(SkeletonCallback *);
    void PostUpdate();

private:
    SkeletonUpdate *mInst; // 0x0

    static CriticalSection sCritSec;
};

class SkeletonUpdate : public SkeletonHistory,
                       public Hmx::Object,
                       public SkeletonHistoryArchive {
    friend class SkeletonUpdateHandle;

public:
    // SkeletonHistory
    virtual bool PrevSkeleton(const Skeleton &, int, ArchiveSkeleton &, int &) const;
    // Hmx::Object
    virtual ~SkeletonUpdate() {}
    virtual bool Replace(ObjRef *, Hmx::Object *);

    static void CreateInstance();
    static void Terminate();
    static bool HasInstance();
    static void *NewSkeletonEvent();
    static SkeletonUpdateHandle InstanceHandle();

private:
    SkeletonUpdate();

    void SetCameraInput(CameraInput *);

    static SkeletonUpdate *sInstance;
    static void *sNewSkeletonEvent;
    static void *sSkeletonUpdatedEvent;

    bool unk78; // 0x78
    ObjOwnerPtr<CameraInput> mCameraInput; // 0x7c
    bool unk90;
    bool unk91;
    std::vector<SkeletonCallback *> mCallbacks; // 0x94
    SkeletonFrame mSkeletonFrame; // 0xa0
    Skeleton mSkeletons[6]; // 0x1268
    int unk[10];
    int unk5388; // 0x5388
    int unk538c; // 0x538c
    bool unk5390; // 0x5390
    int unk5394;
    int unk5398;
    int unk539c;
    int unk53a0;
    int unk53a4;
};
