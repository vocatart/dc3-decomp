#include "rndobj/Rnd.h"
#include "rndobj/Utl.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "rndobj/Bitmap.h"
#include "rndobj/Overlay.h"
#include "rndobj/PostProc.h"
#include "utl/Cheats.h"
#include "utl/FileStream.h"

// Rnd & TheRnd;
bool gNotifyKeepGoing;
bool gFailKeepGoing;
bool gFailRestartConsole;

void Rnd::ShowConsole(bool show) { mConsole->SetShowing(show); }
bool Rnd::ConsoleShowing() { return mConsole->Showing(); }

float Rnd::YRatio() {
    static const float kRatio[5] = { 1.0f, 0.75f, 0.5625f, 0.5625f, 0.6f };
    return kRatio[mAspect];
}

void Rnd::EndWorld() {
    if (!mWorldEnded) {
        if (unk14c) {
        }
        DoWorldEnd();
        DoPostProcess();
        mWorldEnded = true;
    }
}

void Rnd::SetShowTimers(bool show, bool verbose) {
    mTimersOverlay->SetShowing(show);
    mVerboseTimers = verbose;
    SetGSTiming(show);
}

void Rnd::SetProcAndLock(bool b) { mProcCounter.SetProcAndLock(b); }

bool Rnd::ProcAndLock() const { return mProcCounter.ProcAndLock(); }

void Rnd::ResetProcCounter() {
    if (mDrawing) {
        mProcCounter.SetCount(-1);
    } else
        mLastProcCmds = ProcessCmd(mLastProcCmds | kProcessWorld);
}

bool Rnd::GetEvenOddDisabled() const { return mProcCounter.EvenOddDisabled(); }
void Rnd::SetEvenOddDisabled(bool b) { mProcCounter.SetEvenOddDisabled(b); }

void Rnd::ScreenDumpUnique(const char *cc) {
    String filename = UniqueFilename(cc, "bmp");
    ScreenDump(filename.c_str());
}

void Rnd::DrawRectScreen(
    const Hmx::Rect &r,
    const Hmx::Color &c1,
    RndMat *mat,
    const Hmx::Color *cptr1,
    const Hmx::Color *cptr2
) {
    Hmx::Rect rect(r.x * mWidth, r.y * mHeight, r.w * mWidth, r.h * mHeight);
    DrawRect(rect, c1, mat, cptr1, cptr2);
}

Vector2 &Rnd::DrawString(const char *, const Vector2 &v, const Hmx::Color &, bool) {
    static Vector2 s;
    s = v;
    return s;
}

const Vector2 &
Rnd::DrawStringScreen(const char *c, const Vector2 &v, const Hmx::Color &color, bool b4) {
    float fwidth = mWidth;
    float fheight = mHeight;
    Vector2 &vres = DrawString(c, Vector2(v.x * fwidth, v.y * fheight), color, b4);
    vres.x /= fwidth;
    vres.y /= fheight;
    return vres;
}

RndPostProc *Rnd::GetPostProcOverride() { return mPostProcOverride; }

DataNode ModalKeyListener::OnMsg(const KeyboardKeyMsg &k) {
    if (k.GetKey() == 0x12e) {
        if (!GetEnabledKeyCheats() && !TheRnd.ConsoleShowing()) {
            TheRnd.ShowConsole(true);
            return 0;
        } else
            return DataNode(kDataUnhandled, 0);
    } else {
        if (!TheRnd.ConsoleShowing()) {
            gNotifyKeepGoing = true;
            return 0;
        } else
            return DataNode(kDataUnhandled, 0);
    }
}

BEGIN_HANDLERS(ModalKeyListener)
    HANDLE_MESSAGE(KeyboardKeyMsg)
END_HANDLERS

DataNode FailKeepGoing(DataArray *) {
    gFailKeepGoing = true;
    return 0;
}

DataNode FailRestartConsole(DataArray *) {
    gFailRestartConsole = true;
    return 0;
}

RndPostProc *Rnd::GetSelectedPostProc() {
    RndPostProc *selected = nullptr;
    for (std::list<PostProcessor *>::iterator it = mPostProcessors.begin();
         it != mPostProcessors.end();
         ++it) {
        RndPostProc *set = dynamic_cast<RndPostProc *>(*it);
        if (selected) {
            MILO_NOTIFY("More than one postproc selected: %s", PathName(set));
        } else
            selected = set;
    }
    return selected;
}

void Rnd::DoWorldBegin() {
    if (mPostProcBlackLightOverride) {
        mPostProcBlackLightOverride->BeginWorld();
    } else if (mPostProcOverride) {
        mPostProcOverride->BeginWorld();
    } else {
        for (std::list<PostProcessor *>::iterator it = mPostProcessors.begin();
             it != mPostProcessors.end();
             ++it) {
            (*it)->BeginWorld();
        }
    }
}

void Rnd::DoWorldEnd() {
    if (!unk147) {
        CopyWorldCam(nullptr);
    }
    unk147 = false;
    if (mPostProcBlackLightOverride) {
        mPostProcBlackLightOverride->EndWorld();
    } else if (mPostProcOverride) {
        mPostProcOverride->EndWorld();
    } else {
        for (std::list<PostProcessor *>::iterator it = mPostProcessors.begin();
             it != mPostProcessors.end();
             ++it) {
            (*it)->EndWorld();
        }
    }
}

void Rnd::DoPostProcess() {
    if (!mDisablePostProc) {
        if (mPostProcBlackLightOverride) {
            mPostProcBlackLightOverride->DoPost();
        } else if (mPostProcOverride) {
            mPostProcOverride->DoPost();
        } else {
            for (std::list<PostProcessor *>::iterator it = mPostProcessors.begin();
                 it != mPostProcessors.end();
                 ++it) {
                (*it)->DoPost();
            }
        }
    }
}

float Rnd::UpdateOverlay(RndOverlay *o, float f) {
    float ret = f;
    if (o == mRateOverlay) {
        UpdateRate();
    } else if (o == mHeapOverlay) {
        UpdateHeap();
    } else if (o == mWatchOverlay) {
        mWatcher.Update();
    } else if (o == mTimersOverlay) {
        ret = DrawTimers(f);
    }
    return ret;
}

DataNode Rnd::OnShowOverlay(const DataArray *da) {
    RndOverlay *o = RndOverlay::Find(da->Str(2), false);
    if (o) {
        o->SetShowing(da->Int(3));
        if (da->Size() > 4) {
            o->SetTimeout(da->Float(4));
        }
    }
    return 0;
}

DataNode Rnd::OnOverlayPrint(const DataArray *da) {
    RndOverlay *o = RndOverlay::Find(da->Str(2), true);
    String str;
    for (int i = 3; i < da->Size(); i++) {
        da->Evaluate(i).Print(str, true, 0);
    }
    o->Print(str.c_str());
    return 0;
}

DataNode Rnd::OnReflect(const DataArray *da) {
    RndOverlay *o = RndOverlay::Find(da->Sym(2), true);
    if (o->Showing()) {
        TextStream *idk = TheDebug.SetReflect(o);
        for (int i = 3; i < da->Size(); i++) {
            da->Command(i)->Execute(true);
        }
        TheDebug.SetReflect(idk);
    }
    return 0;
}

DataNode Rnd::OnToggleOverlay(const DataArray *da) {
    RndOverlay *o = RndOverlay::Find(da->Str(2), true);
    o->SetShowing(!o->Showing());
    if (o->Showing()) {
        o->SetDumpCount(1);
    }
    return o->Showing();
}

DataNode Rnd::OnToggleOverlayPosition(const DataArray *) {
    RndOverlay::TogglePosition();
    return 0;
}

void Rnd::ScreenDump(const char *file) {
    RndTex *tex = Hmx::Object::New<RndTex>();
    RndBitmap bmap;
    tex->SetBitmap(0, 0, 0, RndTex::kTexFrontBuffer, false, nullptr);
    tex->LockBitmap(bmap, 1);
    FileStream stream(file, FileStream::kWrite, true);
    if (stream.Fail()) {
        MILO_NOTIFY("Screenshot failed; could not open destination file (%s).", file);
    } else {
        bmap.SaveBmp(&stream);
    }
    delete tex;
}

DataNode Rnd::OnShowConsole(const DataArray *) {
    ShowConsole(true);
    return 0;
}

DataNode Rnd::OnToggleTimers(const DataArray *) {
    SetShowTimers(mVerboseTimers || !TimersShowing(), false);
    return 0;
}

DataNode Rnd::OnToggleTimersVerbose(const DataArray *) {
    SetShowTimers(mVerboseTimers == 0, mVerboseTimers == 0);
    return 0;
}

DataNode Rnd::OnClearColorR(const DataArray *) { return mClearColor.red; }
DataNode Rnd::OnClearColorG(const DataArray *) { return mClearColor.green; }
DataNode Rnd::OnClearColorB(const DataArray *) { return mClearColor.blue; }
DataNode Rnd::OnClearColorPacked(const DataArray *) { return mClearColor.Pack(); }

DataNode Rnd::OnSetClearColor(const DataArray *da) {
    SetClearColor(Hmx::Color(da->Float(2), da->Float(3), da->Float(4)));
    return 0;
}

DataNode Rnd::OnSetClearColorPacked(const DataArray *da) {
    SetClearColor(Hmx::Color(
        (da->Int(2) & 255) / 255.0f,
        ((da->Int(2) >> 8) & 255) / 255.0f,
        ((da->Int(2) >> 0x10) & 255) / 255.0f
    ));
    return 0;
}

DataNode Rnd::OnScreenDump(const DataArray *da) {
    ScreenDump(da->Str(2));
    return 0;
}

DataNode Rnd::OnScreenDumpUnique(const DataArray *da) {
    ScreenDumpUnique(da->Str(2));
    return 0;
}

DataNode Rnd::OnScaleObject(const DataArray *da) {
    RndScaleObject(da->GetObj(2), da->Float(3), da->Float(4));
    return 0;
}

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
