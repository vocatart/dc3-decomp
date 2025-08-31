#include "gesture/Skeleton.h"
#include "ArchiveSkeleton.h"
#include "gesture/GestureMgr.h"
#include "gesture/BaseSkeleton.h"
#include "gesture/SkeletonHistory.h"
#include "gesture/JointUtl.h"
#include "math/DoubleExponentialSmoother.h"
#include "obj/Data.h"
#include "os/Debug.h"
#include "os/System.h"

Vector3DESmoother SkeletonFrame::sUpVectorSmoother;

bool Skeleton::IsValid() const {
    if (mSkeletonIdx >= 0) {
        return TheGestureMgr->IsSkeletonValid(mSkeletonIdx);
    } else
        return false;
}

bool Skeleton::IsSitting() const {
    if (mSkeletonIdx >= 0) {
        return TheGestureMgr->IsSkeletonSitting(mSkeletonIdx);
    } else
        return false;
}

bool Skeleton::IsSideways() const {
    if (mSkeletonIdx >= 0) {
        return TheGestureMgr->IsSkeletonSideways(mSkeletonIdx);
    } else
        return false;
}

const TrackedJoint &Skeleton::HandJoint(SkeletonSide side) const {
    return mTrackedJoints[side == kSkeletonLeft ? kJointHandLeft : kJointHandRight];
}

const TrackedJoint &Skeleton::ElbowJoint(SkeletonSide side) const {
    return mTrackedJoints[side == kSkeletonLeft ? kJointElbowLeft : kJointElbowRight];
}

const TrackedJoint &Skeleton::ShoulderJoint(SkeletonSide side) const {
    return mTrackedJoints[side == kSkeletonLeft ? kJointShoulderLeft : kJointShoulderRight];
}

const TrackedJoint &Skeleton::HipJoint(SkeletonSide side) const {
    return mTrackedJoints[side == kSkeletonLeft ? kJointHipLeft : kJointHipRight];
}

const TrackedJoint &Skeleton::KneeJoint(SkeletonSide side) const {
    return mTrackedJoints[side == kSkeletonLeft ? kJointKneeLeft : kJointKneeRight];
}

void Skeleton::CamBoneLengths(float *lens) const {
    memcpy(lens, mCamBoneLengths, sizeof(mCamBoneLengths));
}

void Skeleton::CamJointPositions(Vector3 *positions) const {
    for (int i = 0; i < kNumJoints; i++) {
        *positions++ = mTrackedJoints[i].mJointPos[kCoordCamera];
    }
}

int Skeleton::QualityFlags() const { return mQualityFlags; }

void SkeletonFrame::Init() {
    static Symbol kinect("kinect");
    static Symbol up_vector_smoothing("up_vector_smoothing");
    static Symbol smoothing("smoothing");
    static Symbol trend("trend");
    DataArray *cfg = SystemConfig(kinect, up_vector_smoothing);
    sUpVectorSmoother.SetSmoothParameters(
        cfg->FindFloat(smoothing), cfg->FindFloat(trend)
    );
    sUpVectorSmoother.ForceValue(Vector3(0, 1, 0));
}

void SkeletonFrame::Create(const NUI_SKELETON_FRAME &nui_frame, int i2) {
    unk0 = nui_frame.dwFrameNumber;
    unk4 = i2;
    unk8 = sUpVectorSmoother.Value();
    unk18 = nui_frame.vFloorClipPlane;
}

void Skeleton::JointPos(SkeletonCoordSys cs, SkeletonJoint joint, Vector3 &pos) const {
    MILO_ASSERT((0) <= (cs) && (cs) < (kNumCoordSys), 0xDA);
    MILO_ASSERT((0) <= (joint) && (joint) < (kNumJoints), 0xDB);
    pos = mTrackedJoints[joint].mJointPos[cs];
}

JointConfidence Skeleton::JointConf(SkeletonJoint joint) const {
    MILO_ASSERT((0) <= (joint) && (joint) < (kNumJoints), 0xE1);
    return mTrackedJoints[joint].mJointConf;
}

void Skeleton::CameraToPlayerXfm(SkeletonCoordSys cs, Transform &playerXfm) const {
    MILO_ASSERT((kCoordLeftArm) <= (cs) && (cs) < (kNumCoordSys), 0x127);
    playerXfm = mPlayerXfms[cs - 1];
}

float Skeleton::BoneLength(SkeletonBone bone, SkeletonCoordSys cs) const {
    if (cs == kCoordCamera) {
        MILO_ASSERT((0) <= (bone) && (bone) < (kNumBones), 0x12F);
        return mCamBoneLengths[bone];
    } else
        return BaseSkeleton::BoneLength(bone, cs);
}

void Skeleton::ScreenPos(SkeletonJoint joint, Vector2 &pos) const {
    if (unkaa0 == 2) {
        JointScreenPos(mTrackedJoints[joint], pos);
    } else
        pos.Zero();
}

bool Skeleton::PrevTrackedSkeleton(
    const SkeletonHistory *history, int i2, int &iref, ArchiveSkeleton &archiveSkeleton
) const {
    MILO_ASSERT(history, 0x169);
    if (unkaa0 == 2 && history->PrevSkeleton(*this, i2, archiveSkeleton, iref)) {
        return archiveSkeleton.IsTracked();
    } else
        return false;
}

bool Skeleton::Displacement(
    const SkeletonHistory *history,
    SkeletonCoordSys cs,
    SkeletonJoint joint,
    int i4,
    Vector3 &disp,
    int &iref
) const {
    ArchiveSkeleton archiveSkeleton;
    if (PrevTrackedSkeleton(history, i4, iref, archiveSkeleton)) {
        Vector3 v3;
        archiveSkeleton.JointPos(cs, joint, v3);
        Subtract(mTrackedJoints[joint].mJointPos[cs], v3, disp);
        return true;
    } else {
        disp.Zero();
        return false;
    }
}

bool Skeleton::Velocity(
    const SkeletonHistory &history,
    SkeletonCoordSys cs,
    SkeletonJoint joint,
    int i4,
    Vector3 &velocity,
    int &iref
) const {
    if (Displacement(&history, cs, joint, i4, velocity, iref)) {
        velocity *= (1.0f / (iref * 0.001f));
        return true;
    } else {
        velocity.Zero();
        return false;
    }
}

// bool __thiscall
// Skeleton::Velocity(Skeleton *this,SkeletonHistory *param_1,SkeletonCoordSys param_2,
//                   SkeletonJoint param_3,int param_4,Vector3 *param_5,int *param_6)

// {
//   float fVar1;
//   char cVar2;

//   cVar2 = (**((this->super_BaseSkeleton).vptr +
//   8))(this,param_1,param_2,param_3,param_4); if (cVar2 == '\0') {
//     fVar1 = 0.0;
//     param_5->y = 0.0;
//     param_5->x = 0.0;
//   }
//   else {
//     fVar1 = 1.0 / (*param_6 * 0.001);
//     param_5->x = param_5->x * fVar1;
//     param_5->y = fVar1 * param_5->y;
//     fVar1 = fVar1 * param_5->z;
//   }
//   param_5->z = fVar1;
//   return cVar2 != '\0';
// }

void Skeleton::Init() {
    unkaa0 = 0;
    mSkeletonIdx = -1;
    mQualityFlags = 0;
    unkab0.Zero();
    for (int i = 0; i < 5; i++) {
        mPlayerXfms[i].Reset();
    }
    for (int i = 0; i < kNumJoints; i++) {
        for (int j = 0; j < kNumCoordSys; j++) {
            mTrackedJoints[i].mJointPos[j].Zero();
        }
        mTrackedJoints[i].mJointConf = kConfidenceNotTracked;
        mTrackedJoints[i].unk60.Zero();
    }
    memset(mCamBoneLengths, 0, sizeof(mCamBoneLengths));
    mCamDisplacements.clear();
}

Skeleton::Skeleton() : unkaa0(0), unkaac(-1), unkac4(0) { Init(); }
bool Skeleton::IsTracked() const { return unkaa0 == 2; }
int Skeleton::ElapsedMs() const { return mElapsedMs; }
