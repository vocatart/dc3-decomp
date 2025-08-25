#pragma once
#include "ui/UIListSlot.h"
#include "ui/UIList.h"

/** "Custom slot for use with UIList" */
class UIListSubList : public UIListSlot {
public:
    OBJ_CLASSNAME(UIListSubList)
    OBJ_SET_TYPE(UIListSubList)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    virtual UIList *SubList(int);
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

    static int sNextFillSelection;

protected:
    UIListSubList();

    /** "sub list template" */
    ObjPtr<UIList> mList; // 0x78
};
