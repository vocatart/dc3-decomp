#include "rndobj/Rnd.h"

Rnd::Rnd()
    : mClearColor(0.3f, 0.3f, 0.3f), mWidth(640), mHeight(480), mScreenBpp(16),
      mDrawCount(0), mDrawTimer(), mTimersOverlay(0), mRateOverlay(0), mHeapOverlay(0),
      mWatchOverlay(0), mStatsOverlay(0), mDefaultMat(0), mOverlayMat(0), mOverdrawMat(0),
      mDefaultCam(0), mWorldCamCopy(0), mDefaultEnv(0), mDefaultLit(0), unk110(nullptr),
      unk114(nullptr), unk118(0), unk120(5), mFrameID(0), mRateGate("    "),
      mFont(nullptr), mSync(1), mGsTiming(0), mShowSafeArea(0), mDrawing(0),
      mWorldEnded(1), mAspect(kWidescreen), unk13c(0), unk140(0), unk141(0),
      mShrinkToSafe(1), mInGame(0), mVerboseTimers(0), mDisablePostProc(0), unk146(0),
      unk147(0), unk148(0), unk14c(0), unk150(0), mPostProcOverride(this),
      mPostProcBlackLightOverride(nullptr), unk18c(this), mDraws(this), unk1b4(0),
      mProcCmds(kProcessAll), mLastProcCmds(kProcessAll) {
    for (int i = 0; i < 8; i++)
        mDefaultTex[i] = nullptr;
}
