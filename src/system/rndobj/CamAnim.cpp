#include "rndobj/CamAnim.h"
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "utl/BinStream.h"

BEGIN_HANDLERS(RndCamAnim)
    HANDLE_SUPERCLASS(RndAnimatable)
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

void RndCamAnim::Save(BinStream &bs) {
    bs << 2;
    SAVE_SUPERCLASS(Hmx::Object)
    SAVE_SUPERCLASS(RndAnimatable)
    bs << mCam << mFovKeys << mKeysOwner;
}

bool RndCamAnim::Replace(ObjRef *ref, Hmx::Object *obj) {
    if (&mKeysOwner == ref) {
        if (mKeysOwner == this) {
            mKeysOwner = dynamic_cast<RndCamAnim *>(obj)->KeysOwner();
        } else {
            mKeysOwner = this;
        }
        return true;
    } else
        return Hmx::Object::Replace(ref, obj);
}

void RndCamAnim::SetFrame(float frame, float blend) {
    RndAnimatable::SetFrame(frame, blend);
    if (mCam) {
        if (!FovKeys().empty()) {
            float ref = mCam->YFov();
            FovKeys().AtFrame(frame, ref);
            if (blend != 1) {
                Interp(mCam->YFov(), ref, blend, ref);
            }
            mCam->SetFrustum(mCam->NearPlane(), mCam->FarPlane(), ref, 1.0f);
        }
    }
}

RndCamAnim::~RndCamAnim() {}

RndCamAnim::RndCamAnim() : mCam(this, 0), mKeysOwner(this, this) {}

void RndCamAnim::SetKey(float frame) {
    if (mCam) {
        FovKeys().Add(mCam->YFov(), frame, true);
    }
}

BEGIN_PROPSYNCS(RndCamAnim)
    SYNC_PROP(cam, mCam)
    SYNC_PROP(fov_keys, mFovKeys)
    SYNC_PROP(keys_owner, mKeysOwner)
    SYNC_SUPERCLASS(RndAnimatable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
