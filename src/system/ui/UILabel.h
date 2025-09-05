#pragma once
#include "obj/Object.h"
#include "rndobj/Text.h"
#include "ui/UIComponent.h"
#include "utl/MemMgr.h"

class UILabel : public RndText, public UIComponent, public TextHolder {
public:
    struct LabelStyle {
        LabelStyle(Hmx::Object *);
        int unk0;
        int unk4;
        int unk8;
        int unkc;
        int unk10;
        int unk14;
        int unk18;
        int unk1c;
        int unk20;
        int unk24;
        int unk28;
    };
    // Hmx::Object
    virtual ~UILabel() {}
    OBJ_CLASSNAME(UILabel)
    OBJ_SET_TYPE(UILabel)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    virtual void PreLoad(BinStream &);
    virtual void PostLoad(BinStream &);
    // RndText
    virtual Symbol TextToken();
    virtual void SetCreditsText(DataArray *, class UIListSlot *);
    virtual void SetDisplayText(const char *, bool);
    // UIComponent
    virtual void Poll();
    virtual void OldResourcePreload(BinStream &);
    virtual void Highlight();
    // TextHolder
    virtual void SetTextToken(Symbol);
    virtual void SetInt(int, bool);

    OBJ_MEM_OVERLOAD(0x26);

protected:
    UILabel();

    Symbol unk114; // 0x114
    String unk118; // 0x118
    bool unk120;
    bool unk121;
    bool unk122;
    ObjVector<LabelStyle> unk124; // 0x124
};
