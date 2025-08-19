#include "rndobj/PostProc.h"
#include "os/System.h"
#include "rndobj/DOFProc.h"

void RndPostProc::ResetDofProc() { TheDOFProc->UnSet(); }
RndPostProc *RndPostProc::Current() { return sCurrent; }

ProcCounter::ProcCounter()
    : mProcAndLock(0), mCount(0), mSwitch(0), mOdd(0), mFPS(0), mEvenOddDisabled(0),
      mTriFrameRendering(0) {}

void ProcCounter::SetProcAndLock(bool pandl) {
    mProcAndLock = pandl;
    mCount = -1;
}

void ProcCounter::SetEvenOddDisabled(bool eod) {
    if (mEvenOddDisabled == eod)
        return;
    else
        mEvenOddDisabled = eod;
    if (mEvenOddDisabled)
        mCount = -1;
}

void RndPostProc::Init() {
    sBloomLocFactor = SystemConfig("rnd", "bloom_loc")->FindFloat(SystemLanguage());
}

void RndPostProc::Reset() {
    if (sCurrent) {
        sCurrent->OnUnselect();
        sCurrent = nullptr;
    }
    TheDOFProc->UnSet();
}

RndPostProc::RndPostProc() {}
