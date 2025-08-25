#include "ui/UIPanel.h"

// ObjectDir *UIPanel::DataDir() {
//     if (mDir) {
//         return mDir->DataDir();
//     }
//     return Hmx::Object::DataDir();
// }

void UIPanel::CheckLoad() {
    if (++mLoadRefs == 1)
        Load();
}

bool UIPanel::CheckIsLoaded() {
    if (mState != kUnloaded)
        return true;
    else {
        PollForLoading();
        if (IsLoaded()) {
            FinishLoad();
            return true;
        } else
            return false;
    }
}

UIPanel::UIPanel()
    : mDir(0), mLoader(0), mLoaded(0), mState(kUnloaded), mPaused(0), mShowing(1),
      mForceExit(0), mLoadRefs(0), unk34(0) {
    mPanelId = sMaxPanelId++;
    MILO_ASSERT(sMaxPanelId < 0x8000, 0x27);
}

void UIPanel::SetTypeDef(DataArray *data) {
    Hmx::Object::SetTypeDef(data);
    if (data) {
        static Symbol focus("focus");
        data->FindData(focus, mFocusName, false);
        static Symbol force_exit("force_exit");
        data->FindData(force_exit, mForceExit, false);
    }
}
