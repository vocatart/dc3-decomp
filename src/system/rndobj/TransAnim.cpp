#include "rndobj/TransAnim.h"

RndTransAnim::RndTransAnim()
    : mTrans(this), mTransSpline(false), mScaleSpline(false), mRotSlerp(false),
      mRotSpline(false), mRotKeys(), mTransKeys(), mScaleKeys(), mKeysOwner(this, this),
      mRepeatTrans(false), mFollowPath(false) {}
