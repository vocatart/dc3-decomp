#include "rndobj/PropKeys.h"
#include "math/Rot.h"
#include "math/Utl.h"
#include "os/Debug.h"

void PropKeys::Copy(const PropKeys *keys) {
    mInterpolation = keys->mInterpolation;
    mInterpHandler = keys->mInterpHandler;
    mPropExceptionID = keys->mPropExceptionID;
    unk34 = keys->unk34;
}

int PropKeys::SetKey(float f1) {
    float frame = 0;
    for (int i = 0; i < NumKeys(); i++) {
        if (FrameFromIndex(i, frame) != 0 && NearlyEqual(f1, frame)) {
            return i;
        }
    }
    return -1;
}

void PropKeys::Print() {
    TheDebug << "      target: " << mTarget.Ptr() << "\n";
    TheDebug << "      property: " << mProp << "\n";
    TheDebug << "      interpolation: " << mInterpolation << "\n";

    for (int i = 0; i < NumKeys(); i++) {
        float frame = 0.0f;
        FrameFromIndex(i, frame);
        TheDebug << "      " << frame << " -> ";
        switch (mKeysType) {
        case kFloat:
            TheDebug << (*AsFloatKeys())[i].value;
            break;
        case kColor:
            TheDebug << (*AsColorKeys())[i].value;
            break;
        case kObject:
            TheDebug << (*AsObjectKeys())[i].value;
            break;
        case kBool:
            TheDebug << (*AsBoolKeys())[i].value;
            break;
        case kQuat:
            TheDebug << (*AsQuatKeys())[i].value;
            break;
        case kVector3:
            TheDebug << (*AsVector3Keys())[i].value;
            break;
        case kSymbol:
            TheDebug << (*AsSymbolKeys())[i].value;
            break;
        }
        TheDebug << "\n";
    }
}

PropKeys::~PropKeys() {
    if (mProp) {
        mProp->Release();
        mProp = nullptr;
    }
}

PropKeys::PropKeys(Hmx::Object *targetOwner, Hmx::Object *target)
    : mTarget(targetOwner, target), mProp(nullptr), mKeysType(kFloat),
      mInterpolation(kLinear), mPropExceptionID(kNoException), mTrans(nullptr),
      mLastKeyFrameIndex(-2), unk34(false) {}
