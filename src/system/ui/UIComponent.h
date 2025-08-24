#pragma once
#include "os/User.h"
#include "rndobj/Draw.h"
#include "rndobj/Trans.h"
#include "rndobj/Poll.h"
#include "utl/MemMgr.h"
#include "ui/UIScreen.h"

/**
 * @brief A base implementation of a UI object.
 * Original _objects description:
 * "Base class of all UI components,
 * defines navigation and component state"
 */
class UIComponent : public RndPollable,
                    public virtual RndDrawable,
                    public virtual RndTransformable {
public:
    enum State {
        kNormal = 0,
        kFocused = 1,
        kDisabled = 2,
        kSelecting = 3,
        kSelected = 4,
        kNumStates = 5,
    };
    // Hmx::Object
    virtual ~UIComponent() {}
    OBJ_CLASSNAME(UIComponent)
    OBJ_SET_TYPE(UIComponent)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // RndDrawable
    virtual void Highlight() { RndDrawable::Highlight(); }
    // RndPollable
    virtual void Poll();
    virtual void Enter();
    virtual void Exit();
    virtual void SetState(UIComponent::State);
    virtual Symbol StateSym() const;
    virtual bool Entering() const { return false; }
    virtual bool Exiting() const { return mState == kSelecting; }
    virtual bool CanHaveFocus() { return true; }
    virtual void OldResourcePreload(BinStream &);

    OBJ_MEM_OVERLOAD(0x19);

protected:
    UIComponent();

    State mState; // 0x8
    ObjPtr<UIComponent> mNavRight; // 0xc
    ObjPtr<UIComponent> mNavDown; // 0x20
    LocalUser *mSelectingUser; // 0x34
    UIScreen *mSelectScreen; // 0x38
    int mSelected; // 0x3c
    bool unk40; // 0x40
};
