#pragma once
#include "math/Color.h"
#include "rndobj/Draw.h"
#include "rndobj/Mat.h"
#include "rndobj/Mesh.h"
#include "rndobj/Trans.h"
#include "utl/MemMgr.h"

/** "Line objects represent 3D lines with thickness, perspective, and optional end caps."
 */
class RndLine : public RndDrawable, public RndTransformable {
public:
    class Point {
    public:
        /** "Position of point" */
        Vector3 point; // 0x0
        /** "Color of point" */
        Hmx::Color color; // 0x10
        int unk[10];
    };
    // Hmx::Object
    virtual ~RndLine() { RELEASE(mMesh); }
    OBJ_CLASSNAME(Line);
    OBJ_SET_TYPE(Line);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void Print();
    // RndDrawable
    virtual void UpdateSphere();
    virtual float GetDistanceToPlane(const Plane &, Vector3 &);
    virtual bool MakeWorldSphere(Sphere &, bool);
    virtual void Mats(std::list<class RndMat *> &, bool);
    virtual void DrawShowing();
    virtual RndDrawable *CollideShowing(const Segment &, float &, Plane &);
    virtual int CollidePlane(const Plane &);
    virtual RndMesh *Mesh() const { return mMesh; }
    virtual void Highlight() { RndDrawable::Highlight(); }

    OBJ_MEM_OVERLOAD(0x1A);

protected:
    RndLine();

    void UpdateInternal();

    /** "Width of the line". Ranges from 0 to 1000. */
    float mWidth; // 0x100
    std::vector<Point> mPoints; // 0x104
    RndMesh *mMesh; // 0x110
    /** "The line has end caps" */
    bool mHasCaps; // 0x114
    /** "Consider the [points] as an array of pairs, rather than a continuous line" */
    bool mLinePairs; // 0x115
    /** "Degrees at which the line starts to fold". Ranges from 0 to 180. */
    float mFoldAngle; // 0x118
    float mFoldCos; // 0x11c
    /** "Material the line uses." */
    ObjPtr<RndMat> mMat; // 0x120
    bool unk134;
};
