#pragma once
#include "ResourceDirPtr.h"
#include "rndobj/Dir.h"
#include "ui/UIComponent.h"
#include "ui/ScrollSelect.h"
#include "os/JoypadMsgs.h"
#include "utl/MemMgr.h"

/** "A component with animatable whose frames correspond to a
 *  range of values. The resources don't have to look like a slider;
 *  they could easily be a knob, dial, etc." */
class UISlider : public UIComponent, public ScrollSelect {
public:
    // Hmx::Object
    OBJ_CLASSNAME(UISlider)
    OBJ_SET_TYPE(UISlider)
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
    virtual RndDrawable *CollideShowing(const Segment &, float &, Plane &);
    virtual int CollidePlane(const Plane &);
    // UIComponent
    virtual void Enter();
    virtual void SetCurrent(int);
    // ScrollSelect
    virtual int SelectedAux() const;
    virtual void SetSelectedAux(int);

    OBJ_MEM_OVERLOAD(0x18);

    DataNode OnMsg(const ButtonDownMsg &);

    void SyncSlider();
    float Frame() const;
    void SetNumSteps(int);
    void SetFrame(float);
    int Current() const;

protected:
    UISlider();

    virtual void OldResourcePreload(BinStream &);

    ResourceDirPtr<RndDir> unk50; // 0x50
    int unk68;
    int unk6c;
    int unk70;
    int unk74;
    int unk78;
    int unk7c;
    int mCurrent; // 0x80
    int mNumSteps; // 0x84
    bool mVertical; // 0x88
};
