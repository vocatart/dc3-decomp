#pragma once
#include "obj/Object.h"
#include "rndobj/Anim.h"
#include "rndobj/Mesh.h"
#include "utl/MemMgr.h"

/**
 * @brief A set of RndMesh poses that can be blended between.
 * Commonly used for lipsync.
 * Original _objects description:
 * "A Morph object animates between multiple Mesh poses using
 * weight keyframes. This is an expensive technique, equivalent to a
 * MeshAnim for each active pose, so use only when skinning or a
 * single MeshAnim isn't enough. For example, we use it for
 * viseme-driven facial animation."
 */
class RndMorph : public RndAnimatable {
public:
    struct Pose {
        ObjPtr<RndMesh> mesh;
    };

    // Hmx::Object
    OBJ_CLASSNAME(Morph);
    OBJ_SET_TYPE(Morph);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    // RndAnimatable
    virtual void SetFrame(float frame, float blend);
    virtual float EndFrame();

    OBJ_MEM_OVERLOAD(0x16);

protected:
    RndMorph();

    /** "Number of mesh keyframes to blend". Ranges from 0 to 100. */
    ObjVector<Pose> mPoses; // 0x10
    /** "Mesh for the morph to occur" */
    ObjPtr<RndMesh> mTarget; // 0x20
    /** "Interpolates the normals if set to true, otherwise normals are not affected." */
    bool mNormals; // 0x34
    /** "Smooths the interpolation of the morphing." */
    bool mSpline; // 0x35
    /** "Modifier for weight interpolation" */
    float mIntensity; // 0x38
};
