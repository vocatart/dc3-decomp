#include "obj/Msg.h"
#include "obj/Data.h"
#include "obj/ObjPtr_p.h"
#include "obj/Object.h"
#include "os/Debug.h"

Symbol PathToEventName(DataArray *);

bool MsgSinks::HasPropertySink(Hmx::Object *o, DataArray *a) {
    Symbol path = PathToEventName(a);
    if (unk0) {
        for (int i = 1; i < unk0->Size(); i += 2) {
            if (path == a->Sym(i)) {
                return true;
            }
        }
    }
    return false;
}

bool MsgSinks::HasSink(Hmx::Object *o) const {
    for (ObjList<Sink>::const_iterator it = mSinks.begin(); it != mSinks.end(); ++it) {
        if (it->obj == o) {
            return true;
        }
    }
    return false;
}

void MsgSinks::EventSink::Add(
    Hmx::Object *obj, Hmx::Object::SinkMode mode, Symbol s, bool b4
) {
    EventSinkElem elem(sinks.Owner());
    elem.obj.SetObjConcrete(obj);
    elem.mode = mode;
    elem.handler = s;
    if (b4) {
        sinks.push_front();
        sinks.front() = elem;
    } else {
        sinks.push_back(elem);
    }
}

MsgSinks::~MsgSinks() {
    if (unk0)
        unk0->Release();
}

MsgSinks::MsgSinks(Hmx::Object *o)
    : unk0(nullptr), mSinks(o), mEventSinks(o), mExporting(0), mOwner(o) {}

// BEGIN_CUSTOM_PROPSYNC(MsgSinks::Sink)
//     SYNC_PROP(obj, (Hmx::Object *&)o.obj)
//     SYNC_PROP(mode, (int &)o.mode)
// END_CUSTOM_PROPSYNC

// BEGIN_CUSTOM_PROPSYNC(MsgSinks)
//     SYNC_PROP(sinks, o.mSinks)
//     SYNC_PROP(event_sinks, o.mEventSinks)
// END_PROPSYNCS

void MsgSinks::AddSink(
    Hmx::Object *s, Symbol ev, Symbol handler, Hmx::Object::SinkMode mode, bool chainProxy
) {
    MILO_ASSERT(s, 0x9C);
    if (ev.Null() && !chainProxy) {
        MILO_WARN("%s can't have chainProxy false with no event", PathName(mOwner));
    }
    RemoveSink(s, ev);
    if (ev.Null()) {
        MILO_ASSERT(s != mOwner, 0xA6);
        Sink sink(mOwner);
        sink.obj.SetObjConcrete(s);
        sink.mode = mode;
        if (mExporting != 0) {
            mSinks.push_front(sink);
        } else {
            mSinks.push_back(sink);
        }
    } else {
        if (handler.Null())
            handler = ev;
        MILO_ASSERT((s != mOwner) || (handler != ev), 0xB9);
        for (ObjList<EventSink>::iterator it = mEventSinks.begin();
             it != mEventSinks.end();
             ++it) {
            if (it->event == ev) {
                if (chainProxy != it->chainProxy) {
                    MILO_WARN("%s mismatched proxy chain for %s", PathName(mOwner), ev);
                    mEventSinks.back().Add(s, mode, handler, mExporting);
                    return;
                }
            }
        }
        mEventSinks.push_back();
        mEventSinks.back().event = ev;
        mEventSinks.back().chainProxy = chainProxy;
        mEventSinks.back().Add(s, mode, handler, mExporting);
    }
}
