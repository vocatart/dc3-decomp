#include "gesture/SkeletonUpdate.h"
#include "SkeletonUpdate.h"
#include "gesture/CameraInput.h"
#include "gesture/Skeleton.h"
#include "os/CritSec.h"
#include "os/Debug.h"
#include "utl/Std.h"

CriticalSection SkeletonUpdateHandle::sCritSec;

SkeletonUpdate::SkeletonUpdate()
    : unk78(0), mCameraInput(this), unk90(0), unk91(0), unk5388(0), unk538c(0),
      unk5390(0) {
    MILO_ASSERT(sInstance == NULL, 0x119);
}

SkeletonUpdateHandle::SkeletonUpdateHandle(SkeletonUpdate *update) : mInst(update) {
    MILO_ASSERT(mInst, 0x45);
    sCritSec.Enter();
}

SkeletonUpdateHandle::~SkeletonUpdateHandle() { sCritSec.Exit(); }

std::vector<SkeletonCallback *> &SkeletonUpdateHandle::Callbacks() {
    return mInst->mCallbacks;
}
CameraInput *SkeletonUpdateHandle::GetCameraInput() const { return mInst->mCameraInput; }
void SkeletonUpdateHandle::SetCameraInput(CameraInput *input) {
    mInst->SetCameraInput(input);
}

bool SkeletonUpdateHandle::HasCallback(SkeletonCallback *cb) {
    return VectorFind(mInst->mCallbacks, cb);
}

void SkeletonUpdateHandle::AddCallback(SkeletonCallback *cb) {
    MILO_ASSERT(!HasCallback(cb), 0xA2);
    mInst->mCallbacks.push_back(cb);
}

void SkeletonUpdateHandle::RemoveCallback(SkeletonCallback *cb) {
    MILO_ASSERT(HasCallback(cb), 0xA8);
    mInst->mCallbacks.erase(
        std::find(mInst->mCallbacks.begin(), mInst->mCallbacks.end(), cb)
    );
}

bool SkeletonUpdate::PrevSkeleton(
    const Skeleton &s, int i2, ArchiveSkeleton &as, int &iref
) const {
    return SkeletonHistory::PrevFromArchive(*this, s, i2, as, iref);
}

SkeletonUpdateHandle SkeletonUpdate::InstanceHandle() {
    MILO_ASSERT(sInstance, 0x146);
    return sInstance;
}

void SkeletonUpdate::SetCameraInput(CameraInput *cam_input) {
    MILO_ASSERT(cam_input, 0x165);
    mCameraInput = cam_input;
    for (std::vector<SkeletonCallback *>::iterator it = mCallbacks.begin();
         it != mCallbacks.end();
         ++it) {
        (*it)->Clear();
    }
}

void SkeletonUpdate::CreateInstance() {
    MILO_ASSERT(sInstance == NULL, 0x102);
    sInstance = new SkeletonUpdate();
}

void SkeletonUpdate::Terminate() { RELEASE(sInstance); }
bool SkeletonUpdate::HasInstance() { return sInstance; }
void *SkeletonUpdate::NewSkeletonEvent() { return sNewSkeletonEvent; }
