#include "ui/UIPicture.h"

UIPicture::UIPicture()
    : UITransitionHandler(this), mMesh(this), mTexFile(), mLoadedFile(),
      mTex(Hmx::Object::New<RndTex>()), mLoader(0), mHookTex(true), mDelayedTexFile("") {}

UIPicture::~UIPicture() {
    CancelLoading();
    delete mTex;
}
