#pragma once
#include "obj/Object.h"
#include "ui/UIComponent.h"
#include "utl/Str.h"

class UIPanel : public virtual Hmx::Object {
public:
    enum State {
        kUnloaded = 0,
        kUp = 1,
        kDown = 2,
    };

    UIPanel();
    // Hmx::Object
    virtual ~UIPanel() { Unload(); }
    OBJ_CLASSNAME(UIPanel);
    OBJ_SET_TYPE(UIPanel);
    virtual DataNode Handle(DataArray *, bool);
    virtual void Load(BinStream &bs) { Hmx::Object::Load(bs); }
    virtual void SetTypeDef(DataArray *);
    virtual class ObjectDir *DataDir();
    // UIPanel
protected:
    virtual void Load();

public:
    virtual void Draw();
    virtual void Enter();
    virtual bool Entering() const;
    virtual void Exit();
    virtual bool Exiting() const;
    virtual bool Unloading() const;
    virtual void Poll();
    virtual void SetPaused(bool);
    virtual UIComponent *FocusComponent();
    virtual void FocusIn() {}
    virtual void FocusOut() {}
    virtual bool IsLoaded() const;

    void CheckLoad();
    bool CheckIsLoaded();

protected:
    virtual void Unload();
    virtual void PollForLoading();
    virtual void FinishLoad();

private:
    static int sMaxPanelId;
    static bool sIsFinalDrawPass;

protected:
    class PanelDir *mDir; // 0x8
    DirLoader *mLoader; // 0xc
    String mFocusName; // 0x10
    bool mLoaded; // 0x18
    /** The panel's current state. */
    State mState; // 0x1c
    bool mPaused; // 0x20
    bool mShowing; // 0x21
    bool mForceExit; // 0x22
    /** The number of refs to this loaded UIPanel. */
    int mLoadRefs; // 0x24
    FilePath mFilePath; // 0x28
    /** This panel's ID. */
    int mPanelId; // 0x30
    bool unk34; // 0x34 - something involving a final draw pass
};
