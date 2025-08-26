#include "ui/PanelDir.h"
#include "rndobj/Dir.h"
#include "ui/UIComponent.h"
#include "ui/UITrigger.h"
#include "utl/Loader.h"

bool gSendFocusMsg;

void PanelDir::SyncObjects() {
    RndDir::SyncObjects();
    mComponents.clear();
    for (ObjDirItr<UIComponent> it(this, true); it != nullptr; ++it) {
        mComponents.push_back(it);
    }
    mTriggers.clear();
    for (ObjDirItr<UITrigger> it(this, true); it != nullptr; ++it) {
        mTriggers.push_back(it);
        it->CheckAnims();
    }
    if (sAlwaysNeedFocus) {
        UIComponent *comp = GetFirstFocusableComponent();
        if (!mFocusComponent && comp) {
            gSendFocusMsg = false;
            SetFocusComponent(comp, gNullStr);
            gSendFocusMsg = true;
        }
    }
}

PanelDir::~PanelDir() {
    for (std::vector<RndDir *>::iterator it = mBackPanels.begin();
         it != mBackPanels.end();
         ++it) {
        RELEASE(*it);
    }
    for (std::vector<RndDir *>::iterator it = mFrontPanels.begin();
         it != mFrontPanels.end();
         ++it) {
        RELEASE(*it);
    }
}

PanelDir::PanelDir()
    : mFocusComponent(nullptr), mOwnerPanel(nullptr), mCam(this), mCanEndWorld(true),
      mUseSpecifiedCam(false), mShowEditModePanels(false), mShowFocusComponent(true) {
    if (TheLoadMgr.EditMode()) {
        mShowEditModePanels = true;
    }
}
