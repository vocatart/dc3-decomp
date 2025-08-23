#include "rndobj/Mesh.h"
#include "obj/Data.h"
#include "obj/PropSync.h"
#include "os/Debug.h"
#include "rndobj/Mat.h"
#include "utl/BinStream.h"
#include "utl/Std.h"

int RndMesh::EstimatedSizeKb() const {
    // sizeof(Vert) is 0x50 here
    // but the actual struct is size 0x60
    return (NumVerts() * 0x50 + NumFaces() * sizeof(Face)) / 1024;
}

void RndMesh::ClearCompressedVerts() {
    RELEASE(mCompressedVerts);
    mNumCompressedVerts = 0;
}

BinStream &operator<<(BinStream &bs, const RndMesh::Face &face) {
    bs << face.v1 << face.v2 << face.v3;
    return bs;
}

BinStream &operator<<(BinStream &bs, const RndMesh::Vert &vert) {
    bs << vert.pos << vert.norm;
    bs << vert.color << vert.tex << vert.boneWeights << vert.boneIndices[0]
       << vert.boneIndices[1] << vert.boneIndices[2] << vert.boneIndices[3] << vert.unk50;
    return bs;
}

BEGIN_CUSTOM_PROPSYNC(RndMesh::Vert)
    SYNC_PROP(pos, o.pos)
    SYNC_PROP(norm, o.norm)
    SYNC_PROP(color, o.color)
    SYNC_PROP(alpha, o.color.alpha)
    SYNC_PROP(tex, o.tex)
END_CUSTOM_PROPSYNC

void RndMesh::SetMat(RndMat *mat) { mMat = mat; }

void RndMesh::SetGeomOwner(RndMesh *m) {
    MILO_ASSERT(m, 0x1D7);
    mGeomOwner = m;
}

BEGIN_CUSTOM_PROPSYNC(RndBone)
    SYNC_PROP(bone, o.mBone)
    SYNC_PROP(offset, o.mOffset)
END_CUSTOM_PROPSYNC

void RndMesh::SetKeepMeshData(bool keep) {
    if (keep != mKeepMeshData) {
        mKeepMeshData = keep;
        if (!mKeepMeshData) {
            mVerts.resize(0);
            ClearAndShrink(mFaces);
            ClearAndShrink(mPatches);
        }
    }
}

// void __thiscall RndMesh::SetKeepMeshData(RndMesh *this,bool param_1)

// {
//   Face *pFVar1;
//   int iVar2;
//   uchar *puVar3;
//   StlNodeAlloc<> aSStack_28 [16];

//   if ((param_1 != this[0x171]) && (this[0x171] = param_1, !param_1)) {
//     VertVector::resize(this + 0x100,0);
//     pFVar1 = *(this + 0x110);
//     *(this + 0x110) = 0;
//     *(this + 0x114) = 0;
//     iVar2 = *(this + 0x118);
//     *(this + 0x118) = 0;
//     if (pFVar1 != 0x0) {
//       stlpmtx_std::StlNodeAlloc<>::deallocate(aSStack_28,pFVar1,(iVar2 - pFVar1) / 6);
//     }
//     puVar3 = *(this + 0x130);
//     *(this + 0x130) = 0;
//     *(this + 0x134) = 0;
//     iVar2 = *(this + 0x138);
//     *(this + 0x138) = 0;
//     if (puVar3 != 0x0) {
//       stlpmtx_std::StlNodeAlloc<>::deallocate(aSStack_28,puVar3,iVar2 - puVar3);
//     }
//   }
//   return;
// }

bool PropSync(
    RndMesh ::VertVector &vec, DataNode &node, DataArray *prop, int i, PropOp op
) {
    if (op == kPropUnknown0x40)
        return false;
    else if (i == prop->Size()) {
        MILO_ASSERT(op == kPropSize, 0xA7D);
        node = (int)vec.size();
        return true;
    } else {
        RndMesh::Vert &vert = vec[prop->Int(i++)];
        if (i < prop->Size() || op & (kPropGet | kPropSet | kPropSize)) {
            if (vec.size() > 0) {
                if (op & kPropSet) {
                    vec.unkc = true;
                }
                return PropSync(vert, node, prop, i, op);
            } else {
                MILO_NOTIFY_ONCE(
                    "Cannot modify verts (check if keep_mesh_data is set on the mesh)"
                );
            }
            return true;
        } else {
            MILO_NOTIFY("Cannot add or remove verts of a mesh via property system");
        }
        return true;
    }
}

RndMesh::RndMesh()
    : mMat(this), mGeomOwner(this, this), mBones(this), mMutable(0),
      mVolume(kVolumeTriangles), mBSPTree(nullptr), mMultiMesh(nullptr), unk170(0),
      mKeepMeshData(0), mCompressedVerts(nullptr), mNumCompressedVerts(0) {
    unk180 = 0x26;
}
