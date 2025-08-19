#include "rndobj/PropKeys.h"
#include "math/Rot.h"
#include "math/Utl.h"
#include "obj/Utl.h"
#include "os/Debug.h"
#include "utl/BinStream.h"

Hmx::Object *ObjectStage::sOwner;
Message PropKeys::sInterpMessage(gNullStr, 0, 0, 0, 0, 0);

float CalcSpline(float, const float *);

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

PropKeys::ExceptionID PropKeys::PropExceptionID(Hmx::Object *o, DataArray *arr) {
    if (o && arr) {
        static Symbol rotation("rotation");
        static Symbol scale("scale");
        static Symbol position("position");
        static Symbol event("event");
        if (arr->Size() == 1) {
            Symbol sym = arr->Sym(0);
            if (sym == rotation && IsASubclass(o->ClassName(), "Trans")) {
                return kTransQuat;
            }
            if (sym == scale && IsASubclass(o->ClassName(), "Trans")) {
                return kTransScale;
            }
            if (sym == position && IsASubclass(o->ClassName(), "Trans")) {
                return kTransPos;
            }
            if (sym == event && IsASubclass(o->ClassName(), "ObjectDir")) {
                return kDirEvent;
            }
        }
    }
    return kNoException;
}

void PropKeys::SetPropExceptionID() {
    if (!mInterpHandler.Null())
        mPropExceptionID = kHandleInterp;
    else {
        if (mPropExceptionID != kMacro) {
            mPropExceptionID = PropExceptionID(mTarget.Ptr(), mProp);
            if (mPropExceptionID == kTransQuat || mPropExceptionID == kTransScale
                || mPropExceptionID == kTransPos) {
                if ((Hmx::Object *)mTrans != mTarget.Ptr()) {
                    mTrans = dynamic_cast<RndTransformable *>(mTarget.Ptr());
                }
            }
        }
    }
}

void PropKeys::SetInterpHandler(Symbol sym) {
    mInterpHandler = sym;
    SetPropExceptionID();
}

void FloatKeys::SetToCurrentVal(int i) {
    (*this)[i].value = mTarget->Property(mProp, true)->Float();
}

PropKeys::~PropKeys() {
    if (mProp) {
        mProp->Release();
        mProp = nullptr;
    }
}

void PropKeys::SetProp(DataNode &node) {
    if (node.Type() == kDataArray) {
        DataArray *nodeArr = node.Array();
        if (mProp) {
            mProp->Release();
            mProp = nullptr;
        }
        mProp = nodeArr->Clone(true, false, 0);

    } else
        MILO_NOTIFY("unknown prop set type");
    SetPropExceptionID();
}

void PropKeys::SetTarget(Hmx::Object *o) {
    if (mTarget.Ptr() != o) {
        if ((mProp && GetPropertyVal(o, mProp, false))
            || (mPropExceptionID == kTransQuat || mPropExceptionID == kTransScale
                || mPropExceptionID == kTransPos || mPropExceptionID == kDirEvent)) {
            if (o && mProp) {
                mProp->Release();
                mProp = nullptr;
            }
            mTarget = o;
            SetPropExceptionID();
        }
    }
}

void PropKeys::Save(BinStream &bs) {
    bs << mKeysType;
    bs << mTarget;
    bs << mProp;
    bs << mInterpolation;
    bs << mInterpHandler;
    bs << mPropExceptionID;
    bs << unk34;
}

BinStreamRev &operator>>(BinStreamRev &bs, ObjectStage &stage) {
    ObjectDir *dir = nullptr;
    if (bs.mRev > 8) {
        ObjPtr<ObjectDir> ptr(stage.Owner());
        bs >> ptr;
    }
    bs >> (ObjPtr<Hmx::Object> &)stage;
    return bs;
}

BinStream &operator<<(BinStream &bs, const ObjectStage &stage) {
    ObjPtr<ObjectDir> dirPtr(stage.Owner(), (stage.Ptr()) ? stage.Ptr()->Dir() : nullptr);
    bs << dirPtr;
    bs << ObjPtr<Hmx::Object>(stage);
    return bs;
}

PropKeys::PropKeys(Hmx::Object *targetOwner, Hmx::Object *target)
    : mTarget(targetOwner, target), mProp(nullptr), mKeysType(kFloat),
      mInterpolation(kLinear), mPropExceptionID(kNoException), mTrans(nullptr),
      mLastKeyFrameIndex(-2), unk34(false) {}

void PropKeys::Load(BinStreamRev &bs) {
    if (bs.mRev < 7)
        MILO_FAIL("PropKeys::Load should not be called before version 7");
    else {
        int iVal;
        bs >> iVal;
        mKeysType = (AnimKeysType)iVal;
        bs >> mTarget;
        bs >> mProp;

        if (bs.mRev >= 8)
            bs >> iVal;
        else if (mKeysType == kObject || mKeysType == kBool)
            iVal = 0;
        else
            iVal = 1;

        if (bs.mRev < 11 && iVal == 4) {
            mPropExceptionID = kMacro;
            mInterpolation = kStep;
        } else
            mInterpolation = (Interpolation)iVal;

        if (bs.mRev > 9) {
            Symbol sym;
            bs >> sym;
            if (!sym.Null()) {
                SetInterpHandler(sym);
            }
        }

        if (bs.mRev > 10) {
            bs >> iVal;
            mPropExceptionID = (ExceptionID)iVal;
        }

        if (bs.mRev > 0xC) {
            bs >> unk34;
        }
        SetPropExceptionID();
    }
}

void FloatKeys::SetFrame(float frame, float f2, float f3) {
    if (mProp && mTarget && size()) {
        int idx;
        if (mPropExceptionID != kHandleInterp) {
            float val;
            idx = FloatAt(frame, val);
            mTarget->SetProperty(mProp, val * f3);
        } else {
            const Key<float> *prev;
            const Key<float> *next;
            float ref = 0;
            idx = AtFrame(frame, prev, next, ref);
            sInterpMessage.SetType(mInterpHandler);
            sInterpMessage[0] = prev->value * f3;
            sInterpMessage[1] = next->value * f3;
            sInterpMessage[2] = ref;
            sInterpMessage[3] = next->frame;
            if (idx >= 1)
                sInterpMessage[4] = (*this)[idx - 1].value * f3;
            else
                sInterpMessage[4] = 0;
            mTarget->Handle(sInterpMessage, true);
        }
        mLastKeyFrameIndex = idx;
    }
}

void FloatKeys::Copy(const PropKeys *keys) {
    PropKeys::Copy(keys);
    clear();
    if (keys->KeysType() == mKeysType) {
        const FloatKeys *newKeys = dynamic_cast<const FloatKeys *>(keys);
        insert(begin(), newKeys->begin(), newKeys->end());
    }
}

int FloatKeys::FloatAt(float frame, float &fl) {
    MILO_ASSERT(size(), 0x188);
    fl = 0.0f;
    float ref = 0.0f;
    const Key<float> *prev;
    const Key<float> *next;
    int at = AtFrame(frame, prev, next, ref);
    switch (mInterpolation) {
    case kStep:
        fl = prev->value;
        break;
    case kLinear:
        Interp(prev->value, next->value, ref, fl);
        break;
    case kSpline:
        if (size() < 3 || prev == next) {
            Interp(prev->value, next->value, ref, fl);
        } else {
            float points[4];
            points[1] = prev->value;
            points[2] = next->value;
            int idx = (prev - begin());
            if (idx == 0) {
                points[0] = prev->value;
            } else {
                points[0] = this->at(idx - 1).value;
            }
            if (idx == size() - 1) {
                points[3] = next->value;
            } else {
                points[3] = this->at(idx + 1).value;
            }
            fl = CalcSpline(ref, points);
        }
        break;
    case kHermite:
        Interp(prev->value, next->value, ref * ref * (ref * -2.0f + 3.0f), fl);
        break;
    case kEaseIn:
        Interp(prev->value, next->value, ref * ref * ref, fl);
        break;
    case kEaseOut:
        ref = 1.0f - ref;
        Interp(prev->value, next->value, -(ref * ref * ref - 1.0f), fl);
        break;
    }
    return at;
}

int FloatKeys::SetKey(float frame) {
    if (!mProp || !mTarget.Ptr())
        return -1;
    else {
        int retVal = PropKeys::SetKey(frame);
        if (retVal < 0)
            retVal = Add(0, frame, false);
        SetToCurrentVal(retVal);
        return retVal;
    }
}

void PropKeys::ReSort() {
    switch (mKeysType) {
    case kFloat:
        std::sort(AsFloatKeys()->begin(), AsFloatKeys()->end());
        break;
    case kColor:
        std::sort(AsColorKeys()->begin(), AsColorKeys()->end());
        break;
    case kObject:
        std::sort(AsObjectKeys()->begin(), AsObjectKeys()->end());
        break;
    case kBool:
        std::sort(AsBoolKeys()->begin(), AsBoolKeys()->end());
        break;
    case kSymbol:
        std::sort(AsSymbolKeys()->begin(), AsSymbolKeys()->end());
        break;
    case kVector3:
        std::sort(AsVector3Keys()->begin(), AsVector3Keys()->end());
        break;
    case kQuat:
        std::sort(AsQuatKeys()->begin(), AsQuatKeys()->end());
        break;
    }
}

void PropKeys::ChangeFrame(int idx, float new_frame, bool sort) {
    switch (mKeysType) {
    case kFloat:
        (*AsFloatKeys())[idx].frame = new_frame;
        break;
    case kColor:
        (*AsColorKeys())[idx].frame = new_frame;
        break;
    case kObject:
        (*AsObjectKeys())[idx].frame = new_frame;
        break;
    case kBool:
        (*AsBoolKeys())[idx].frame = new_frame;
        break;
    case kSymbol:
        (*AsSymbolKeys())[idx].frame = new_frame;
        break;
    case kVector3:
        (*AsVector3Keys())[idx].frame = new_frame;
        break;
    case kQuat:
        (*AsQuatKeys())[idx].frame = new_frame;
        break;
    default:
        MILO_NOTIFY("can not replace frame, unknown type");
        break;
    }
    if (sort)
        ReSort();
}
