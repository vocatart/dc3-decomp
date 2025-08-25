#include "ui/UIListState.h"

UIListState::UIListState(UIListProvider *provider, UIListStateCallback *callback)
    : mCircular(0), mNumDisplay(5), mGridSpan(1), mSpeed(0.25f), mMinDisplay(0),
      mScrollPastMinDisplay(0), mMaxDisplay(-1), mScrollPastMaxDisplay(1),
      mProvider(provider), mFirstShowing(0), mTargetShowing(0), mSelectedDisplay(0),
      mStepPercent(0.0f), mStepTime(-1.0f), mCallback(callback) {}
