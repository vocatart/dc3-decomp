#include "ui/UITransitionHandler.h"

UITransitionHandler::~UITransitionHandler() {
    if (mInAnim)
        mInAnim->StopAnimation();
    if (mOutAnim)
        mOutAnim->StopAnimation();
}

UITransitionHandler::UITransitionHandler(Hmx::Object *obj)
    : mInAnim(obj), mOutAnim(obj), mAnimationState(kUITransitionAnimationInvalid),
      mChangePending(0), unk31(0) {}
