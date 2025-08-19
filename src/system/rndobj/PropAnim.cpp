#include "rndobj/PropAnim.h"
#include "utl/Std.h"

RndPropAnim::RndPropAnim()
    : mLastFrame(0), mInSetFrame(false), mLoop(false), unk20(""), unk24(1) {}

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
