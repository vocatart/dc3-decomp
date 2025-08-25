#include "ui/PanelDir.h"
#include "utl/Loader.h"

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
