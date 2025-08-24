#pragma once
#include "obj/Object.h"
#include "rndobj/Mesh.h"

class RndMeshDeform : public Hmx::Object {
public:
    class VertArray {
    public:
        VertArray(RndMeshDeform *md) : mSize(0), mData(nullptr), mParent(md) {}
        ~VertArray();
        void Clear();
        int NumVerts();
        void *FindVert(int);
        void CopyVert(int, int, VertArray &);
        void AppendWeights(int, int *, float *);
        void SetSize(int);
        void Copy(const VertArray &);
        void Load(BinStream &);

        int mSize;
        void *mData;
        RndMeshDeform *mParent;
    };

    // size 0x6c
    class BoneDesc {
    public:
        char pad[0x6c];
    };

    virtual ~RndMeshDeform();
    OBJ_CLASSNAME(MeshDeform);
    OBJ_SET_TYPE(MeshDeform);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void PreSave(BinStream &);
    virtual void Print();

protected:
    RndMeshDeform();

    ObjPtr<RndMesh> mMesh; // 0x1c
    Transform mMeshInverse; // 0x28
    ObjVector<BoneDesc> mBones; // 0x58
    VertArray mVerts; // 0x68
    bool mSkipInverse;
    bool mDeformed;
};
