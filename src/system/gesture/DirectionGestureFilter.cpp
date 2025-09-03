#include "gesture/DirectionGestureFilter.h"
#include "BaseSkeleton.h"
#include "gesture/BaseSkeleton.h"
#include "rndobj/Overlay.h"

DirectionGestureFilterSingleUser::DirectionGestureFilterSingleUser(
    SkeletonSide s1, SkeletonSide s2, float f3, float f4
)
    : unk4(s1), unkc(s2), unk14(0), unk18(0), mEngaged(0), mAllowAboveShoulder(1),
      mHighButtonMode(0), unk20(0.5) {
    mConfidence = kConfidenceNotTracked;
    ClearSwipe();
    SkeletonJoint wristJoint, elbowJoint;
    if (s1 == kSkeletonRight) {
        elbowJoint = kJointElbowRight;
        wristJoint = kJointWristRight;
    } else {
        elbowJoint = kJointElbowLeft;
        wristJoint = kJointWristLeft;
    }
    mArcDetector.Initialize(s2, wristJoint, elbowJoint, f3);
}

DirectionGestureFilterSingleUser::~DirectionGestureFilterSingleUser() {
    RndOverlay *swipeOverlay = RndOverlay::Find("swipe_direction", false);
    if (swipeOverlay && swipeOverlay->GetCallback() == this) {
        swipeOverlay->SetCallback(nullptr);
    }
}

void DirectionGestureFilterSingleUser::ResetHoverTimer() {
    mArcDetector.ResetHoverTimer();
}

void DirectionGestureFilterSingleUser::Clear() {
    mConfidence = kConfidenceNotTracked;
    ClearSwipe();
}

void DirectionGestureFilterSingleUser::ClearSwipe() {
    unk14 = 0;
    mHasDirection = false;
    unk18 = 0;
    mArcDetector.Clear();
}

bool DirectionGestureFilterSingleUser::IsLockedIn() const {
    return mArcDetector.IsLockedIn();
}

float DirectionGestureFilterSingleUser::UpdateOverlay(RndOverlay *overlay, float f1) {
    return mArcDetector.UpdateOverlay(overlay, f1);
}

// float __thiscall
// DirectionGestureFilterSingleUser::UpdateOverlay
//           (DirectionGestureFilterSingleUser *this,RndOverlay *param_1,float param_2)

// {
//   float fVar1;

//   fVar1 = ArcDetector::UpdateOverlay(this + 0x24,param_1,param_2);
//   return fVar1;
// }
