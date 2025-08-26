#include "ui/UI.h"
#include "obj/Data.h"
#include "obj/DataFile.h"
#include "os/Debug.h"
#include "ui/UIScreen.h"
#include "ui/UIPanel.h"
#include "utl/Str.h"
#include "utl/Symbol.h"

const char *TransitionStateString(UIManager::TransitionState s) {
    switch (s) {
    case UIManager::kTransitionTo:
        return "to";
    case UIManager::kTransitionFrom:
        return "from";
    case UIManager::kTransitionPop:
        return "pop";
    default:
        return "";
    }
}

void UIManager::SetScreenBlacklghtDisabled(bool disable) {
    mDisableScreenBlacklight = disable;
}

bool UIManager::InComponentSelect() {
    if (mCurrentScreen)
        return mCurrentScreen->InComponentSelect();
    else
        return false;
}

UIPanel *UIManager::FocusPanel() {
    if (mCurrentScreen)
        return mCurrentScreen->FocusPanel();
    else
        return nullptr;
}

UIComponent *UIManager::FocusComponent() {
    UIPanel *focusPanel = FocusPanel();
    if (focusPanel)
        return focusPanel->FocusComponent();
    else
        return nullptr;
}

void UIManager::ToggleLoadTimes() {
    mOverlay->CurrentLine() = gNullStr;
    mOverlay->SetShowing(!mOverlay->Showing());
}

void Automator::StartAuto(UIScreen *screen) {
    MILO_ASSERT(mScreenScripts, 0xC0);
    mCurScript = nullptr;
    if (screen) {
        mCurMsgIndex = 1;
        for (int i = mCurScreenIndex; i < mScreenScripts->Size(); i++) {
            DataArray *arr = mScreenScripts->Array(i);
            if (arr->Sym(0) == screen->Name()) {
                mCurScript = arr;
                mCurScreenIndex++;
                break;
            }
        }
    }
}

Symbol Automator::CurRecordScreen() {
    DataArray *recordArr = mRecord;
    if (recordArr->Size() > 0) {
        return recordArr->Array(recordArr->Size() - 1)->Sym(0);
    } else
        return gNullStr;
}

void TerminateCallback() {
    MILO_ASSERT(TheUI, 0x1CE);
    TheUI->Terminate();
}

void FailAppendCallback(FixedString &str) {
    if ((TheUI && TheUI->CurrentScreen()) || TheUI->TransitionScreen()) {
        str += "\n";
        if (TheUI->CurrentScreen()) {
            str += "Screen: ";
            str += TheUI->CurrentScreen()->Name();
        }
        if (TheUI->InTransition()) {
            str += "Trans: ";
            str += TransitionStateString(TheUI->GetTransitionState());
            str += " ";
            str += TheUI->TransitionScreen()->Name();
        }
    }
}

void UIManager::GotoFirstScreen() {
    GotoScreen(DataVariable("first_screen").Obj<UIScreen>(), false, false);
    mTimer.Restart();
}

void UIManager::Draw() {
    for (std::vector<UIScreen *>::iterator it = mPushedScreens.begin();
         it != mPushedScreens.end();
         ++it) {
        (*it)->Draw();
    }
    if (mCurrentScreen)
        mCurrentScreen->Draw();
}

void UIManager::GotoScreen(const char *name, bool b2, bool b3) {
    UIScreen *screen = ObjectDir::Main()->Find<UIScreen>(name, true);
    MILO_ASSERT(screen, 0x37E);
    GotoScreen(screen, b2, b3);
}

void UIManager::ResetScreen(UIScreen *screen) {
    if (mTransitionState != kTransitionNone && mTransitionState != kTransitionFrom) {
        bool old = mCancelTransitionNotify;
        mCancelTransitionNotify = false;
        CancelTransition();
        mCancelTransitionNotify = old;
    }
    if (mPushedScreens.empty()) {
        GotoScreen(screen, false, false);
    } else {
        MILO_ASSERT(mPushedScreens.size() == 1, 0x3E5);
        PopScreen(screen);
    }
}

UIScreen *UIManager::BottomScreen() {
    return !mPushedScreens.empty() ? mPushedScreens.front() : mCurrentScreen;
}

int UIManager::PushDepth() const { return mPushedScreens.size(); }

UIScreen *UIManager::ScreenAtDepth(int depth) {
    MILO_ASSERT(depth < mPushedScreens.size(), 0x46F);
    return mPushedScreens[depth];
}

const char *Automator::ToggleAuto() {
    mCurScript = 0;
    if (mScreenScripts) {
        mScreenScripts->Release();
        mScreenScripts = 0;
    } else {
        Loader *ldr = TheLoadMgr.AddLoader(mAutoPath.c_str(), kLoadFront);
        DataLoader *dl = dynamic_cast<DataLoader *>(ldr);
        MILO_ASSERT(dl, 0x90);
        TheLoadMgr.PollUntilLoaded(dl, 0);
        mScreenScripts = dl->Data();
        mCurScreenIndex = 0;
        if (mScreenScripts) {
            StartAuto(mUIManager.CurrentScreen());
        }
    }
    return AutoScript();
}

void Automator::AddRecord(Symbol s, DataArray *arr) {
    MILO_ASSERT(mRecord, 0x14F);
    int recordSize = mRecord->Size();
    DataArray *addArr;
    if (CurRecordScreen() == s) {
        addArr = mRecord->Array(recordSize - 1);
    } else {
        addArr = new DataArray(1);
        addArr->Node(0) = s;
        mRecord->Insert(recordSize, addArr);
    }
    addArr->Insert(addArr->Size(), arr);
}

void Automator::FinishRecord() {
    if (mRecord) {
        MILO_ASSERT(!mRecordPath.empty(), 0x162);
        DataWriteFile(mRecordPath.c_str(), mRecord, 0);
    }
    if (mRecord) {
        mRecord->Release();
        mRecord = nullptr;
    }
}

UIManager::UIManager() {}

Automator::Automator(UIManager &mgr)
    : mUIManager(mgr), mScreenScripts(0), mRecord(0), mAutoPath("automator.dta"),
      mRecordPath("automator.dta"), mCurScript(0), mSkipNextQuickCheat(0) {}

Automator::~Automator() {
    if (mScreenScripts) {
        mScreenScripts->Release();
        mScreenScripts = 0;
    }
    FinishRecord();
}

DataNode Automator::OnCustomMsg(const Message &msg) {
    Symbol key = msg.Type();
    // ain't no way this is how hmx wrote it
    std::list<Symbol>::iterator it = mCustomMsgs.begin();
    if (it != mCustomMsgs.end()) {
        for (; it != mCustomMsgs.end() && *it != key; ++it)
            ;
        if (it != mCustomMsgs.end())
            HandleMessage(key);
    }
    return DataNode(kDataUnhandled, 0);
}
