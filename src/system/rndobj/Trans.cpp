#include "rndobj/Trans.h"
#include "math/Mtx.h"
#include "rndobj/Utl.h"
#include "obj/Data.h"
#include "obj/ObjMacros.h"
#include "os/Debug.h"
#include "utl/BinStream.h"
#include "math/Rot.h"

void RndTransformable::SetDirty_Force() {
    mDirty = true;
    if (!mChildren.empty()) {
        for (std::list<RndTransformable *>::iterator it = mChildren.begin();
             it != mChildren.end();
             ++it) {
            (*it)->SetDirty();
        }
    }
}

namespace {
    bool HorizontalCmp(const RndTransformable *t1, const RndTransformable *t2) {
        return t1->LocalXfm().v[0] < t2->LocalXfm().v[0];
    }

    bool VerticalCmp(const RndTransformable *t1, const RndTransformable *t2) {
        return t1->LocalXfm().v[2] > t2->LocalXfm().v[2];
    }
}

void RndTransformable::Save(BinStream &bs) {
    bs << 9;
    if (ClassName() == StaticClassName()) {
        SAVE_SUPERCLASS(Hmx::Object)
    }
    bs << mLocalXfm;
    bs << mWorldXfm;
    bs << mConstraint;
    bs << mTarget;
    bs << mPreserveScale;
    bs << mParent;
}

DataNode RndTransformable::OnGetLocalPos(const DataArray *da) {
    *da->Var(2) = mLocalXfm.v.x;
    *da->Var(3) = mLocalXfm.v.y;
    *da->Var(4) = mLocalXfm.v.z;
    return 0;
}

DataNode RndTransformable::OnGetLocalPosIndex(const DataArray *a) {
    MILO_ASSERT(a->Int(2) < 3, 0x351);
    return mLocalXfm.v[a->Int(2)];
}

DataNode RndTransformable::OnGetLocalRot(const DataArray *a) {
    Vector3 v;
    GetLocalRot(v);
    *a->Var(2) = v.x;
    *a->Var(3) = v.y;
    *a->Var(4) = v.z;
    return 0;
}

DataNode RndTransformable::OnGetLocalRotIndex(const DataArray *a) {
    MILO_ASSERT(a->Int(2) < 3, 0x36B);
    Vector3 v1, v2;
    MakeEulerScale(mLocalXfm.m, v1, v2);
    v1 *= RAD2DEG;
    return v1[a->Int(2)];
}

DataNode RndTransformable::OnSetLocalScale(const DataArray *a) {
    SetLocalScale(this, Vector3(a->Float(2), a->Float(3), a->Float(4)));
    return 0;
}

DataNode RndTransformable::OnSetLocalScaleIndex(const DataArray *a) {
    MILO_ASSERT(a->Int(2) < 3, 0x3C1);
    Vector3 v28;
    MakeScale(LocalXfm().m, v28);
    v28[a->Int(2)] = a->Float(3);
    SetLocalScale(this, v28);
    return 0;
}

DataNode RndTransformable::OnGetLocalScale(const DataArray *da) {
    Vector3 v20;
    MakeScale(LocalXfm().m, v20);
    *da->Var(2) = v20.x;
    *da->Var(3) = v20.y;
    *da->Var(4) = v20.z;
    return 0;
}

DataNode RndTransformable::OnGetLocalScaleIndex(const DataArray *a) {
    MILO_ASSERT(a->Int(2) < 3, 0x3D9);
    Vector3 v28;
    MakeScale(LocalXfm().m, v28);
    return v28[a->Int(2)];
}

void RndTransformable::SetWorldXfm(const Transform &xfm) {
    mWorldXfm = xfm;
    mDirty = false;
    UpdatedWorldXfm();
    for (std::list<RndTransformable *>::iterator it = mChildren.begin();
         it != mChildren.end();
         ++it) {
        (*it)->SetDirty();
    }
}

void RndTransformable::SetWorldPos(const Vector3 &pos) {
    mWorldXfm.v = pos;
    UpdatedWorldXfm();
    for (std::list<RndTransformable *>::iterator it = mChildren.begin();
         it != mChildren.end();
         ++it) {
        (*it)->SetDirty();
    }
}

void RndTransformable::SetTransConstraint(
    Constraint cst, RndTransformable *t, bool preserveScale
) {
    MILO_ASSERT(t != this, 0x164);
    mConstraint = cst;
    mPreserveScale = preserveScale;
    mTarget = t;
    SetDirty();
}

DataNode RndTransformable::OnCopyLocalTo(const DataArray *da) {
    DataArray *arr = da->Array(2);
    for (int i = arr->Size() - 1; i >= 0; i--) {
        RndTransformable *t = arr->Obj<RndTransformable>(i);
        t->SetLocalXfm(mLocalXfm);
    }
    return 0;
}

DataNode RndTransformable::OnSetLocalPos(const DataArray *da) {
    SetLocalPos(Vector3(da->Float(2), da->Float(3), da->Float(4)));
    return 0;
}

DataNode RndTransformable::OnSetLocalPosIndex(const DataArray *a) {
    MILO_ASSERT(a->Int(2) < 3, 0x385);
    Vector3 v28(mLocalXfm.v);
    v28[a->Int(2)] = a->Float(3);
    SetLocalPos(v28);
    return 0;
}

void RndTransformable::SetLocalRot(Vector3 v) {
    v *= DEG2RAD;
    Hmx::Matrix3 m;
    MakeRotMatrix(v, m, true);
    SetLocalRot(m);
}

DataNode RndTransformable::OnSetLocalRot(const DataArray *da) {
    SetLocalRot(Vector3(da->Float(2), da->Float(3), da->Float(4)));
    return 0;
}

DataNode RndTransformable::OnSetLocalRotMat(const DataArray *da) {
    Hmx::Matrix3 m(
        da->Float(2),
        da->Float(3),
        da->Float(4),
        da->Float(5),
        da->Float(6),
        da->Float(7),
        da->Float(8),
        da->Float(9),
        da->Float(10)
    );
    SetLocalRot(m);
    return 0;
}

DataNode RndTransformable::OnSetTransConstraint(const DataArray *da) {
    RndTransformable *trans = 0;
    if (da->Size() > 3)
        trans = da->Obj<RndTransformable>(3);
    SetTransConstraint((Constraint)da->Int(2), trans, false);
    return 0;
}

DataNode RndTransformable::OnGetChildren(const DataArray *da) {
    DataArray *arr = new DataArray((int)mChildren.size());
    int idx = 0;
    for (std::list<RndTransformable *>::iterator it = mChildren.begin();
         it != mChildren.end();
         ++it) {
        arr->Node(idx++) = *it;
    }
    DataNode ret(arr, kDataArray);
    arr->Release();
    return ret;
}

RndTransformable::RndTransformable()
    : mParent(this), mTarget(this), mConstraint(kConstraintNone), mPreserveScale(false),
      mDirty(true) {
    mLocalXfm.Reset();
    mWorldXfm.Reset();
}

RndTransformable::~RndTransformable() {
    for (std::list<RndTransformable *>::iterator it = mChildren.begin();
         it != mChildren.end();
         ++it) {
        RndTransformable *cur = *it;
        cur->mParent = nullptr;
        cur->SetDirty();
    }
}
