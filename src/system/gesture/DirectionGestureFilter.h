#pragma once
#include "ArcDetector.h"
#include "gesture/BaseSkeleton.h"
#include "gesture/Skeleton.h"
#include "rndobj/Overlay.h"
#include "gesture/SkeletonViz.h"

class DirectionGestureFilter : public RndOverlay::Callback {
public:
    virtual ~DirectionGestureFilter() {}
};

class DirectionGestureFilterSingleUser : public DirectionGestureFilter {
public:
    DirectionGestureFilterSingleUser(SkeletonSide, SkeletonSide, float, float);
    virtual ~DirectionGestureFilterSingleUser();
    virtual void Clear();
    virtual JointConfidence Confidence() const { return mConfidence; }
    virtual void Update(const Skeleton &, int);
    virtual void Draw(const Skeleton &, SkeletonViz &);
    virtual bool HasDirection() const { return mHasDirection; }
    virtual float GetPercentPulled() const { return unk18; }
    virtual bool IsHandValid(const Skeleton &) const;
    virtual bool IsValidScrollPos(const Skeleton &) const;
    virtual void ClearSwipe();
    virtual bool IsLockedIn() const;
    virtual void SetEngaged(bool engaged) { mEngaged = engaged; }
    virtual void ResetHoverTimer();
    virtual void SetAllowAboveShoulder(bool allow) { mAllowAboveShoulder = allow; }
    virtual void SetHighButtonMode(bool set) { mHighButtonMode = set; }

private:
    virtual float UpdateOverlay(RndOverlay *, float);

protected:
    SkeletonSide unk4;
    JointConfidence mConfidence; // 0x8
    SkeletonSide unkc;
    bool mHasDirection; // 0x10
    float unk14;
    float unk18;
    bool mEngaged; // 0x1c
    bool mAllowAboveShoulder; // 0x1d
    bool mHighButtonMode; // 0x1e
    float unk20;
    ArcDetector mArcDetector; // 0x24
};
