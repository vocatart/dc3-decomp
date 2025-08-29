#include "char/CharBonesMeshes.h"
#include "obj/Object.h"

CharBonesMeshes::CharBonesMeshes() : mMeshes(this, (EraseMode)0, kObjListOwnerControl) {}

CharBonesMeshes::~CharBonesMeshes() { mMeshes.clear(); }

void CharBonesMeshes::ReallocateInternal() {
    CharBonesAlloc::ReallocateInternal();
    mMeshes.clear();
    mMeshes.reserve(mBones.size());
}
