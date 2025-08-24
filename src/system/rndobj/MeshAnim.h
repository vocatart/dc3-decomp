#pragma once
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "rndobj/Mesh.h"

class RndMeshAnim : public RndAnimatable {
public:
    // Hmx::Object
    virtual bool Replace(ObjRef *, Hmx::Object *);
    OBJ_CLASSNAME(MeshAnim);
    OBJ_SET_TYPE(MeshAnim);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    // RndAnimatable
    virtual void SetFrame(float, float);
    virtual float EndFrame();

    Keys<std::vector<Vector3>, std::vector<RndMesh::Vert> > &VertPointsKeys() {
        return mKeysOwner->mVertPointsKeys;
    }
    Keys<std::vector<Vector3>, std::vector<RndMesh::Vert> > &VertNormalsKeys() {
        return mKeysOwner->mVertNormalsKeys;
    }
    Keys<std::vector<Vector2>, std::vector<RndMesh::Vert> > &VertTexsKeys() {
        return mKeysOwner->mVertTexsKeys;
    }
    Keys<std::vector<Hmx::Color>, std::vector<RndMesh::Vert> > &VertColorsKeys() {
        return mKeysOwner->mVertColorsKeys;
    }
    RndMeshAnim *KeysOwner() const { return mKeysOwner; }

protected:
    RndMeshAnim();

    ObjPtr<RndMesh> mMesh; // 0x10
    Keys<std::vector<Vector3>, std::vector<RndMesh::Vert> > mVertPointsKeys; // 0x24
    Keys<std::vector<Vector3>, std::vector<RndMesh::Vert> > mVertNormalsKeys; // 0x30
    Keys<std::vector<Vector2>, std::vector<RndMesh::Vert> > mVertTexsKeys; // 0x3c
    Keys<std::vector<Hmx::Color>, std::vector<RndMesh::Vert> > mVertColorsKeys; // 0x48
    ObjOwnerPtr<RndMeshAnim> mKeysOwner; // 0x54
};
