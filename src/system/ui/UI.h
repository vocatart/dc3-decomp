#pragma once
#include "obj/Object.h"
#include "os/Timer.h"
#include "rndobj/Cam.h"
#include "rndobj/Env.h"
#include "rndobj/Overlay.h"
#include "ui/UIScreen.h"

class Automator;
class JoypadClient;

enum TransitionState {
    kTransitionNone = 0,
    kTransitionTo = 1,
    kTransitionFrom = 2,
    kTransitionPop = 3,
};

class UIManager : public Hmx::Object {
public:
    UIManager();
    virtual ~UIManager();
    virtual DataNode Handle(DataArray *, bool);
    virtual void Init();
    virtual void Terminate();
    virtual void Poll();
    virtual void Draw();
    virtual void GotoScreen(UIScreen *, bool, bool);
    virtual void GotoScreen(const char *, bool, bool);
    virtual void PushScreen(UIScreen *);
    virtual void PopScreen(UIScreen *);
    virtual void ResetScreen(UIScreen *);
    virtual bool InComponentSelect();
    virtual bool IsBlockingTransition() { return false; }
    virtual bool IsTimelineResetAllowed() const { return true; }

protected:
    TransitionState mTransitionState; // 0x2c
    bool mWentBack; // 0x30
    std::vector<UIScreen *> mPushedScreens; // 0x34
    int mMaxPushDepth; // 0x40
    JoypadClient *mJoyClient; // 0x44
    UIScreen *mCurrentScreen; // 0x48
    int unk4c;
    int unk50;
    RndCam *mCam; // 0x54
    RndEnviron *mEnv; // 0x58
    int unk5c;
    Timer mTimer; // 0x60
    bool mOverloadHorizontalNav; // 0x90
    bool mCancelTransitionNotify; // 0x91
    bool mDefaultAllowEditText; // 0x92
    bool unk93;
    int unk94;
    Timer mLoadTimer; // 0x98
    RndOverlay *mOverlay; // 0xc8
    Automator *mAutomator; // 0xcc
    bool unkd0;
};

extern UIManager *TheUI;

class Automator : public Hmx::Object {
public:
    Automator(UIManager &);
    virtual ~Automator() {}
    virtual DataNode Handle(DataArray *, bool);
};
