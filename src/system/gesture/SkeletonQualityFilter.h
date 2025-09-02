#pragma once
#include "gesture/Skeleton.h"

class SkeletonQualityFilter {
public:
    SkeletonQualityFilter();
    virtual ~SkeletonQualityFilter() {}

    void Init(float, float);
    void SetSidewaysCutoffThreshold(float);
    void RestoreDefaultSidewaysCutoffThreshold();
    void Update(const Skeleton &, bool);

private:
    void UpdateIsConfident(const TrackedJoint *);
    void UpdateIsSideways(const TrackedJoint *);
    void UpdateIsSitting(const TrackedJoint *);

    float unk4;
    float unk8;
    bool unkc;
    float unk10;
    bool unk14;
    bool unk15;
    bool unk16;
    float mSidewaysCutoffThreshold; // 0x18
};
