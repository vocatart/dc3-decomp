#include "ui/Screenshot.h"

Screenshot::~Screenshot() {
    RELEASE(mTex);
    RELEASE(mMat);
}

Screenshot::Screenshot() : mTex(nullptr), mMat(nullptr) {}
