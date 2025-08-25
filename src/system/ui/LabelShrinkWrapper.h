#pragma once
#include "rndobj/Dir.h"
#include "ui/ResourceDirPtr.h"
#include "ui/UIComponent.h"

/** "a mesh shrink wrapped to selected label" */
class LabelShrinkWrapper : public UIComponent {
public:
    // Hmx::Object
    OBJ_CLASSNAME(LabelShrinkWrapper)
    OBJ_SET_TYPE(LabelShrinkWrapper)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void SetTypeDef(DataArray *);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // RndDrawable
    virtual void DrawShowing();
    // RndPollable
    virtual void Poll();
    virtual void Enter();
    // UIComponent
    virtual void OldResourcePreload(BinStream &);

protected:
    LabelShrinkWrapper();

    ResourceDirPtr<RndDir> unk44; // 0x44
};
