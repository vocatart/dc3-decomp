#pragma once
#include "obj/Object.h"

class UIPanel;

struct PanelRef {};

class UIScreen : public Hmx::Object {
public:
    UIScreen();
    OBJ_CLASSNAME(UIScreen);
    OBJ_SET_TYPE(UIScreen);
    virtual DataNode Handle(DataArray *, bool);
    virtual void SetTypeDef(DataArray *);
    virtual void LoadPanels();
    virtual void UnloadPanels();
    virtual bool CheckIsLoaded();
    virtual bool IsLoaded() const;
    virtual void Poll();
    virtual void Draw();
    virtual bool InComponentSelect() const;
    virtual void Enter(UIScreen *);
    virtual bool Entering() const;
    virtual void Exit(UIScreen *);
    virtual bool Exiting() const;
    virtual void Print(TextStream &);
    virtual bool Unloading() const;

    UIPanel *FocusPanel() const { return mFocusPanel; }

protected:
    static int sMaxScreenId;

    std::list<PanelRef> mPanelList; // 0x2c
    UIPanel *mFocusPanel; // 0x34
    DataArray *mBack; // 0x38
    bool mClearVram; // 0x3c
    bool mShowing; // 0x3d
    int mScreenId; // 0x40
};
