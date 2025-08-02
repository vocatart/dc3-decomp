#include "obj/Msg.h"
#include "Object.h"

MsgSinks::~MsgSinks() {
    if (unk0)
        unk0->Release();
}

MsgSinks::MsgSinks(Hmx::Object *o)
    : unk0(nullptr), mSinks(o), mEventSinks(o), unk1c(0), mOwner(o) {}

// BEGIN_CUSTOM_PROPSYNC(MsgSinks::Sink)
//     SYNC_PROP(obj, (Hmx::Object *&)o.obj)
//     SYNC_PROP(mode, (int &)o.mode)
// END_CUSTOM_PROPSYNC

// BEGIN_CUSTOM_PROPSYNC(MsgSinks)
//     SYNC_PROP(sinks, o.mSinks)
//     SYNC_PROP(event_sinks, o.mEventSinks)
// END_PROPSYNCS
