#pragma once
#include "flow/FlowNode.h"

class FlowQueueable : public FlowNode {
public:
    enum InterruptType {
        /** "If we're activated, ignore the activation" */
        kIgnore = 0,
        /** "New activations go in the queue and are executed when this one finishes" */
        kQueue = 1,
        /** "New activations go into a one deep queue and are executed when this one
         * finishes" */
        kQueueOne = 2,
        /** "Forcably stop what we're doing and restart" */
        kImmediate = 3,
        /** "Ask our children to stop, then run again when they finish" */
        kWhenAble = 4
    };

    // Hmx::Object
    virtual ~FlowQueueable();
    OBJ_CLASSNAME(FlowQueueable)
    OBJ_SET_TYPE(FlowQueueable)
    virtual DataNode Handle(DataArray *, bool);
    virtual bool SyncProperty(DataNode &, DataArray *, int, PropOp);
    virtual void Save(BinStream &);
    virtual void Copy(const Hmx::Object *, CopyType);
    virtual void Load(BinStream &);
    // FlowNode
    virtual void Deactivate(bool);
    virtual void ChildFinished(FlowNode *);
    virtual void RequestStop();
    virtual void RequestStopCancel();
    virtual bool ActivateTrigger() { return FlowNode::Activate(); }
    virtual bool Activate(Hmx::Object *);

    OBJ_MEM_OVERLOAD(0x1A)
    NEW_OBJ(FlowQueueable)

    void ReleaseListener(Hmx::Object *);

protected:
    FlowQueueable();

    /** "Determines how we handle re-triggering of this label" */
    InterruptType mInterrupt; // 0x5c
    std::list<Hmx::Object *> unk60; // 0x60
};
