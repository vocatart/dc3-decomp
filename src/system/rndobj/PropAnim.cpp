#include "rndobj/PropAnim.h"
#include "obj/Data.h"
#include "utl/Std.h"

void RndPropAnim::AdvanceFrame(float frame) {
    if (mLoop) {
        frame = ModRange(StartFrame(), EndFrame(), frame);
    }
    RndAnimatable::SetFrame(frame, 1.0f);
}

float RndPropAnim::StartFrame() {
    float frame = 0.0f;
    for (std::list<PropKeys *>::iterator it = mPropKeys.begin(); it != mPropKeys.end();
         ++it) {
        frame = Min(frame, (*it)->StartFrame());
    }
    return frame;
}

float RndPropAnim::EndFrame() {
    float frame = 0.0f;
    for (std::list<PropKeys *>::iterator it = mPropKeys.begin(); it != mPropKeys.end();
         ++it) {
        frame = Max(frame, (*it)->EndFrame());
    }
    return frame;
}

void RndPropAnim::SetKey(float frame) {
    for (std::list<PropKeys *>::iterator it = mPropKeys.begin(); it != mPropKeys.end();
         ++it) {
        (*it)->SetKey(frame);
    }
}

void RndPropAnim::StartAnim() {
    for (std::list<PropKeys *>::iterator it = mPropKeys.begin(); it != mPropKeys.end();
         ++it) {
        (*it)->ResetLastKeyFrameIndex();
    }
}

void RndPropAnim::Print() {
    int idx = 0;
    for (std::list<PropKeys *>::iterator it = mPropKeys.begin(); it != mPropKeys.end();
         ++it) {
        TheDebug << "   Keys " << idx << "\n";
        (*it)->Print();
        idx++;
    }
}

DataNode RndPropAnim::OnListFlowLabels(DataArray *arr) {
    int i = 0;
    if (!mFlowLabels.empty()) {
        DataArray *flowArr = new DataArray(mFlowLabels.size());
        for (std::list<String>::iterator it = mFlowLabels.begin();
             it != mFlowLabels.end();
             ++it, ++i) {
            flowArr->Node(i) = Symbol(it->c_str());
        }
        DataNode ret = DataNode(flowArr, kDataArray);
        flowArr->Release();
        return ret;
    } else {
        DataArray *flowArr = new DataArray(1);
        flowArr->Node(0) = Symbol();
        DataNode ret = DataNode(flowArr, kDataArray);
        flowArr->Release();
        return ret;
    }
}

RndPropAnim::RndPropAnim()
    : mLastFrame(0), mInSetFrame(false), mLoop(false), mFireFlowLabel(""), mIntensity(1) {
}

RndPropAnim::~RndPropAnim() { DeleteAll(mPropKeys); }

PropKeys *
RndPropAnim::AddKeys(Hmx::Object *obj, DataArray *prop, PropKeys::AnimKeysType ty) {
    PropKeys *theKeys = GetKeys(obj, prop);
    if (theKeys)
        return theKeys;
    else {
        switch (ty) {
        case PropKeys::kFloat:
            theKeys = new FloatKeys(this, obj);
            break;
        case PropKeys::kColor:
            theKeys = new ColorKeys(this, obj);
            break;
        case PropKeys::kObject:
            theKeys = new ObjectKeys(this, obj);
            break;
        case PropKeys::kBool:
            theKeys = new BoolKeys(this, obj);
            break;
        case PropKeys::kQuat:
            theKeys = new QuatKeys(this, obj);
            break;
        case PropKeys::kVector3:
            theKeys = new Vector3Keys(this, obj);
            break;
        case PropKeys::kSymbol:
            theKeys = new SymbolKeys(this, obj);
            break;
        default:
            MILO_NOTIFY("Unable to create animation for keysType");
            return nullptr;
        }
        if (prop) {
            DataNode node(prop, kDataArray);
            theKeys->SetProp(node);
        }
        mPropKeys.push_back(theKeys);
    }
    return theKeys;
}
