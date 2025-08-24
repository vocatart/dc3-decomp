#include "rndobj/AmbientOcclusion.h"
#include "obj/Object.h"

RndAmbientOcclusion::RndAmbientOcclusion()
    : mDontCastAO(this), mDontReceiveAO(this), mTessellate(this) {}

BEGIN_PROPSYNCS(RndAmbientOcclusion)
    SYNC_PROP(dont_cast_ao, mDontCastAO)
    SYNC_PROP(dont_receive_ao, mDontReceiveAO)
    SYNC_PROP(tessellate, mTessellate)
    SYNC_PROP(ignore_transparent, mIgnoreTransparent)
    SYNC_PROP(ignore_prelit, mIgnorePrelit)
    SYNC_PROP(ignore_hidden, mIgnoreHidden)
    SYNC_PROP(use_mesh_normals, mUseMeshNormals)
    SYNC_PROP(intersect_back_faces, mIntersectBackFaces)
    SYNC_PROP(tessellate_tri_limit, mTessellateTriLimit)
    SYNC_PROP(tessellate_tri_error, mTessellateTriError)
    SYNC_PROP(tessellate_tri_large, mTessellateTriLarge)
    SYNC_PROP(tessellate_tri_small, mTessellateTriSmall)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
