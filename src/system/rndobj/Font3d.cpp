#include "rndobj/Font.h"

bool RndFont3d::HasChar(unsigned short us) const {
    return mCharInfoMap.find(us) != mCharInfoMap.end();
}

RndFont3d::RndFont3d()
    : unk44(this), mTextureOwner(this, this), unk6c(0, 0, 0), unk7c(0, 0, 0),
      unk8c(0, 0, 0) {}

void RndFont3d::Clear() {
    for (std::map<unsigned short, CharInfo *>::iterator it = mCharInfoMap.begin();
         it != mCharInfoMap.end();
         ++it) {
        delete it->second;
    }
    mCharInfoMap.clear();
    mChars.clear();
    RELEASE(mKerningTable);
}
