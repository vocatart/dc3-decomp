#pragma once
#include "ui/UIComponent.h"

class InlineHelp : public UIComponent {
public:
    // Hmx::Object
    OBJ_CLASSNAME(InlineHelp);
    OBJ_SET_TYPE(InlineHelp);
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
    virtual void OldResourcePreload(BinStream &);

protected:
    InlineHelp();

    virtual void SyncLabelsToConfig();
    virtual void UpdateIconTypes(bool);
    virtual String GetIconStringFromAction(int);
};
