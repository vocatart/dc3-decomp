#include "ui/UILabelDir.h"
#include "ui/UIComponent.h"

UILabelDir::UILabelDir()
    : mDefaultColor(this), mFocusAnim(this), mPulseAnim(this),
      mFocusedBackgroundGroup(this), mUnfocusedBackgroundGroup(this),
      mAllowEditText(false) {
    for (int i = 0; i < UIComponent::kNumStates; i++) {
        mColors.push_back(ObjPtr<UIColor>(this));
    }
}
