#pragma once
#include "math/Geo.h"
#include "math/Vec.h"
#include "obj/Object.h"
#include "rndobj/Mesh.h"
#include "utl/MemMgr.h"

/** "Computes ambient occlusion and automatic tessellation.
    Also stores AO configuration options." */
class RndAmbientOcclusion : public Hmx::Object {
public:
    // Hmx::Object
    virtual ~RndAmbientOcclusion() { Clean(); }
    OBJ_CLASSNAME(AmbientOcclusion);
    OBJ_SET_TYPE(AmbientOcclusion);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

    void Clean();

    OBJ_MEM_OVERLOAD(0x15);

protected:
    RndAmbientOcclusion();

    /** "These objects will NOT cast shadows." */
    ObjPtrList<Hmx::Object> mDontCastAO; // 0x2c
    /** "These objects will NOT receive shadows." */
    ObjPtrList<Hmx::Object> mDontReceiveAO; // 0x40
    /** "These objects will be automatically tessellated." */
    ObjPtrList<Hmx::Object> mTessellate; // 0x54
    /** "Ignore transparent objects when casting shadows." */
    bool mIgnoreTransparent; // 0x68
    /** "Ignore prelit materials when receiving shadows." */
    bool mIgnorePrelit; // 0x69
    /** "Ignore hidden objects when casting or receiving shadows." */
    bool mIgnoreHidden; // 0x6a
    /** "Use the mesh normals when calculating. Otherwise,
        smoothed normals will be re-generated for each mesh." */
    bool mUseMeshNormals; // 0x6b
    /** "If true, this will cause back faces of triangles to be added.
        This will more than double the calculation time." */
    bool mIntersectBackFaces; // 0x6c
    /** "The maximum number of polys tessellation will generate.
        This is a multiplier of the current poly count.
        A value of 2 will generate a maximum of 2x the polys."
        Ranges from 2 to 32. */
    int mTessellateTriLimit; // 0x70
    /** "The error threshold for the ambient occlusion calculation.
        Error larger than this value will result in a triangle being split."
        Ranges from 0.1 to 15.0. */
    float mTessellateTriError; // 0x74
    /** "Triangles larger than this size will always be split."
        Ranges from 1 to 500. */
    float mTessellateTriLarge; // 0x78
    /** "Triangles smaller than this size will not be split any further."
        Ranges from 0.1 to 500. */
    float mTessellateTriSmall; // 0x7c
    std::vector<RndMesh *> unk80;
    std::vector<RndMesh *> unk8c;
    std::vector<RndMesh *> unk98;
    std::vector<Triangle> unka4;
    int unkb0;
    int unkb4;
    std::vector<Vector3> unkb8;
};
