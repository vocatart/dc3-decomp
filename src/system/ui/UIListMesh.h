#pragma once
#include "ui/UIListSlot.h"
#include "rndobj/Mat.h"
#include "rndobj/Mesh.h"

/** "Custom slot for use with UIList" */
class UIListMesh : public UIListSlot {
public:
    OBJ_CLASSNAME(UIListMesh)
    OBJ_SET_TYPE(UIListMesh)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual void Draw(
        const UIListWidgetDrawState &,
        const UIListState &,
        const Transform &,
        UIComponent::State,
        Box *,
        DrawCommand
    );
    virtual UIListSlotElement *CreateElement(UIList *);
    virtual RndTransformable *RootTrans();

    RndMat *DefaultMat() const;
    RndMesh *Mesh() const { return mMesh; }

protected:
    UIListMesh();

    /** "mesh to draw/transform" */
    ObjPtr<RndMesh> mMesh; // 0x78
    /** "default material" */
    ObjPtr<RndMat> mDefaultMat; // 0x8c
};
