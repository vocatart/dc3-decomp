#pragma once
#include "obj/Object.h"
#include "rndobj/Anim.h"

enum UITransitionAnimationState {
    kUITransitionAnimationInvalid,
    kUITransitionAnimationIdle,
    kUITransitionAnimationInAnimating,
    kUITransitionAnimationOutAnimating,
    kUITransitionAnimationReverseOutAnimating
};

class UITransitionHandler {
public:
    UITransitionHandler(Hmx::Object *);
    virtual ~UITransitionHandler();
    virtual void FinishValueChange();
    virtual void StartValueChange();
    virtual bool IsEmptyValue() const = 0;

    /** "animation kicked off before [transition]" */
    ObjPtr<RndAnimatable> mInAnim; // 0x4
    /** "animation kicked off after [transition]" */
    ObjPtr<RndAnimatable> mOutAnim; // 0x18
    /** The current transition animation state. */
    UITransitionAnimationState mAnimationState; // 0x2c
    bool mChangePending; // 0x30
    bool unk31; // 0x31
};
