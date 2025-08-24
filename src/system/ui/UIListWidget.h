#pragma once
#include "obj/Object.h"
#include "ui/UIColor.h"

enum UIListWidgetState {
    kUIListWidgetActive,
    kUIListWidgetHighlight,
    kUIListWidgetInactive,
    kNumUIListWidgetStates
};

enum UIListWidgetDrawType {
    kUIListWidgetDrawAlways,
    kUIListWidgetDrawOnlyFocused,
    kUIListWidgetDrawFocusedOrManual,
    kNumUIListWidgetDrawTypes
};

class UIListWidget : public Hmx::Object {
public:
    // Hmx::Object
    virtual ~UIListWidget() {}
    OBJ_CLASSNAME(UIListWidget)
    OBJ_SET_TYPE(UIListWidget)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);

protected:
    UIListWidget();

    float mDrawOrder; // 0x1c
    float mDisabledAlphaScale; // 0x20
    ObjPtr<UIColor> mDefaultColor; // 0x24
};
