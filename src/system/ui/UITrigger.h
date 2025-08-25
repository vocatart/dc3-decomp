#pragma once
#include "rndobj/EventTrigger.h"
#include "rndobj/Poll.h"

/** "Triggers anims based on UI events (enter, exit, etc.)" */
class UITrigger : public EventTrigger, public RndPollable {
public:
    // Hmx::Object
    OBJ_CLASSNAME(UITrigger);
    OBJ_SET_TYPE(UITrigger);
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, Hmx::Object::CopyType);
    virtual void Load(BinStream &);
    // EventTrigger
    virtual void Trigger();
    virtual DataArray *SupportedEvents();
    virtual void CheckAnims();
    // RndPollable
    virtual void Poll();
    virtual void Enter();

protected:
    UITrigger();

    bool unk11c; // 0x11c
    ObjPtr<Hmx::Object> mCallbackObject; // 0x120
    int unk134;
    float unk138;
    bool unk13c;
};
