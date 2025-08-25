#pragma once
#include "ui/UIComponent.h"
#include "ui/UILabel.h"

/** "a ticker to control counting up or down for a given number based label" */
class LabelNumberTicker : public UIComponent {
public:
    // Hmx::Object
    virtual ~LabelNumberTicker();
    OBJ_CLASSNAME(LabelNumberTicker)
    OBJ_SET_TYPE(LabelNumberTicker)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // RndPollable
    virtual void Poll();
    virtual void Enter();

protected:
    LabelNumberTicker();

    /** "label to be shrink wrapped" */
    ObjPtr<UILabel> mLabel; // 0x44
};
