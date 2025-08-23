#include "rndobj/EventTrigger.h"
#include "obj/Data.h"
#include "obj/DataFunc.h"
#include "os/Debug.h"
#include "os/System.h"
#include "rndobj/Anim.h"
#include "utl/BinStream.h"

DataArray *gSupportedEvents;

void EventTrigger::StartAnim() {
    mFrame = kHugeFloat;
    if (mAnimTrigger == kTriggerAnimStart) {
        Trigger();
    }
}

void EventTrigger::EndAnim() {
    mFrame = kHugeFloat;
    if (mAnimTrigger == kTriggerAnimEnd) {
        Trigger();
    } else if (mAnimTrigger == kTriggerAnimStart) {
        BasicReset();
    }
}

void EventTrigger::SetFrame(float frame, float blend) {
    float oldframe = mFrame;
    RndAnimatable::SetFrame(frame, blend);
    if (mAnimTrigger == kTriggerAnimFrame && oldframe < mAnimFrame
        && mFrame >= mAnimFrame) {
        Trigger();
    }
}

DataArray *EventTrigger::SupportedEvents() {
    DataArray *cfg;
    if (Type() == "endgame_action") {
        cfg = SystemConfig(
            "objects", "EventTrigger", "types", "endgame_action", "supported_events"
        );
    } else {
        cfg = SystemConfig("objects", "EventTrigger", "supported_events");
    }
    gSupportedEvents = cfg->Array(1);
    return gSupportedEvents;
}

void ResetAnim(EventTrigger::Anim &anim) {
    if (anim.mAnim) {
        anim.mRate = anim.mAnim->GetRate();
        anim.mStart = anim.mAnim->StartFrame();
        anim.mEnd = anim.mAnim->EndFrame();
        anim.mPeriod = 0.0f;
        anim.mScale = 1.0f;
        static Symbol range("range");
        static Symbol loop("loop");
        anim.mType = anim.mAnim->Loop() ? loop : range;
    }
}

BinStream &operator<<(BinStream &bs, const EventTrigger::HideDelay &e) {
    bs << e.mHide << e.mDelay << e.mRate;
    return bs;
}

BinStream &operator<<(BinStream &bs, const EventTrigger::Anim &e) {
    bs << e.mAnim << e.mBlend << e.mWait << e.mDelay;
    bs << e.mEnable;
    bs << e.mRate << e.mStart;
    bs << e.mEnd << e.mPeriod;
    bs << e.mType;
    bs << e.mScale;
    return bs;
}

BinStream &operator<<(BinStream &bs, const EventTrigger::ProxyCall &e) {
    bs << e.mProxy;
    bs << e.mCall;
    bs << e.mEvent;
    return bs;
}

void EventTrigger::RegisterEvents() {
    Hmx::Object *src = Dir();
    if (src) {
        static Symbol trigger("trigger");
        for (std::list<Symbol>::iterator it = mTriggerEvents.begin();
             it != mTriggerEvents.end();
             ++it) {
            src->AddSink(this, *it, trigger);
        }
        static Symbol enable("enable");
        for (std::list<Symbol>::iterator it = mEnableEvents.begin();
             it != mEnableEvents.end();
             ++it) {
            src->AddSink(this, *it, enable);
        }
        static Symbol disable("disable");
        for (std::list<Symbol>::iterator it = mDisableEvents.begin();
             it != mDisableEvents.end();
             ++it) {
            src->AddSink(this, *it, disable);
        }
        static Symbol wait_for("wait_for");
        for (std::list<Symbol>::iterator it = mWaitForEvents.begin();
             it != mWaitForEvents.end();
             ++it) {
            src->AddSink(this, *it, wait_for);
        }
        mEnabled = mEnabledAtStart;
    }
}

void EventTrigger::UnregisterEvents() {
    Hmx::Object *src = Dir();
    if (src) {
        for (std::list<Symbol>::iterator it = mTriggerEvents.begin();
             it != mTriggerEvents.end();
             ++it) {
            src->RemoveSink(this, *it);
        }
        for (std::list<Symbol>::iterator it = mEnableEvents.begin();
             it != mEnableEvents.end();
             ++it) {
            src->RemoveSink(this, *it);
        }
        for (std::list<Symbol>::iterator it = mDisableEvents.begin();
             it != mDisableEvents.end();
             ++it) {
            src->RemoveSink(this, *it);
        }
        for (std::list<Symbol>::iterator it = mWaitForEvents.begin();
             it != mWaitForEvents.end();
             ++it) {
            src->RemoveSink(this, *it);
        }
    }
}

void EventTrigger::CleanupEventCase(std::list<Symbol> &syms) {
    for (std::list<Symbol>::iterator it = syms.begin(); it != syms.end(); ++it) {
        const char *lightStr = strstr(it->Str(), "lighting_");
        if (lightStr) {
            String str(*it);
            str.ToLower();
            *it = str.c_str();
        }
    }
}

DataNode EventTrigger::OnTrigger(DataArray *) {
    if (mEnabled) {
        if (!mWaitForEvents.empty()) {
            mWaiting = true;
        } else
            Trigger();
    }
    return 0;
}

void EventTrigger::SetName(const char *cc, class ObjectDir *dir) {
    UnregisterEvents();
    Hmx::Object::SetName(cc, dir);
    RegisterEvents();
}

EventTrigger::Anim::Anim(Hmx::Object *o)
    : mAnim(o), mBlend(0), mDelay(0), mWait(0), mEnable(0), mRate(k30_fps), mStart(0),
      mEnd(0), mPeriod(0), mScale(1) {
    static Symbol range("range");
    mType = range;
}

EventTrigger::ProxyCall::ProxyCall(Hmx::Object *o) : mProxy(o), mEvent(o) {}
EventTrigger::HideDelay::HideDelay(Hmx::Object *o) : mHide(o, 0), mDelay(0), mRate(0) {}

void RemoveNullEvents(std::list<Symbol> &vec) {
    std::list<Symbol>::iterator it = vec.begin();
    while (it != vec.end()) {
        if (it->Null())
            it = vec.erase(it);
        else
            it++;
    }
}

BEGIN_HANDLERS(EventTrigger)
    HANDLE(trigger, OnTrigger)
    HANDLE_ACTION(enable, mEnabled = true)
    HANDLE_ACTION(disable, mEnabled = false)
    HANDLE_ACTION_IF(wait_for, mWaiting, Trigger())
    HANDLE(proxy_calls, OnProxyCalls)
    HANDLE_EXPR(supported_events, DataNode(SupportedEvents(), kDataArray))
    HANDLE_ACTION(basic_cleanup, BasicReset())
    HANDLE_SUPERCLASS(RndAnimatable)
    HANDLE_SUPERCLASS(Hmx::Object)
END_HANDLERS

void EventTrigger::SetNextLink(EventTrigger *trig) {
    for (EventTrigger *it = trig; it != nullptr; it = it->mNextLink) {
        if (it == this) {
            MILO_NOTIFY(
                "Setting next link causes infinite loop, setting next_link to NULL"
            );
            mNextLink = nullptr;
            return;
        }
    }
    mNextLink = trig;
}

BEGIN_CUSTOM_PROPSYNC(EventTrigger::Anim)
    SYNC_PROP_MODIFY(anim, o.mAnim, ResetAnim(o))
    SYNC_PROP(blend, o.mBlend)
    SYNC_PROP(wait, o.mWait)
    SYNC_PROP(delay, o.mDelay)
    SYNC_PROP(enable, o.mEnable)
    SYNC_PROP(rate, (int &)o.mRate)
    SYNC_PROP(start, o.mStart)
    SYNC_PROP(end, o.mEnd)
    SYNC_PROP(scale, o.mScale)
    SYNC_PROP(period, o.mPeriod)
    SYNC_PROP(type, o.mType)
END_CUSTOM_PROPSYNC

BEGIN_CUSTOM_PROPSYNC(EventTrigger::ProxyCall)
    SYNC_PROP_MODIFY(proxy, o.mProxy, o.mCall = "")
    SYNC_PROP(call, o.mCall)
    SYNC_PROP(event, o.mEvent)
END_CUSTOM_PROPSYNC

BEGIN_CUSTOM_PROPSYNC(EventTrigger::HideDelay)
    SYNC_PROP(hide, o.mHide)
    SYNC_PROP(delay, o.mDelay)
    SYNC_PROP(rate, o.mRate)
END_CUSTOM_PROPSYNC

void EventTrigger::Save(BinStream &bs) {
    bs << 0x11;
    SAVE_SUPERCLASS(Hmx::Object)
    SAVE_SUPERCLASS(RndAnimatable)
    bs << mTriggerEvents << mAnims << mSounds << mShows << mHideDelays;
    bs << mEnableEvents << mDisableEvents << mWaitForEvents;
    bs << mNextLink << mProxyCalls << mTriggerOrder << mResetTriggers;
    bs << mEnabledAtStart << mAnimTrigger << mAnimFrame;
    bs << mPartLaunchers;
}

void EventTrigger::CleanupHideShow() {
    mTriggered = false;
    mShown.clear();
    mHidden.clear();
}

#define SYNC_PROP_TRIGGER(s, member)                                                     \
    {                                                                                    \
        _NEW_STATIC_SYMBOL(s)                                                            \
        if (sym == _s) {                                                                 \
            if (!(_op & (kPropSize | kPropGet)))                                         \
                UnregisterEvents();                                                      \
            if (PropSync(member, _val, _prop, _i + 1, _op)) {                            \
                if (!(_op & (kPropSize | kPropGet))) {                                   \
                    RegisterEvents();                                                    \
                }                                                                        \
                return true;                                                             \
            } else {                                                                     \
                return false;                                                            \
            }                                                                            \
        }                                                                                \
    }

BEGIN_PROPSYNCS(EventTrigger)
    SYNC_PROP_TRIGGER(trigger_events, mTriggerEvents)
    SYNC_PROP_MODIFY(anims, mAnims, CheckAnims())
    SYNC_PROP(proxy_calls, mProxyCalls)
    SYNC_PROP(sounds, mSounds)
    SYNC_PROP(shows, mShows)
    SYNC_PROP(hide_delays, mHideDelays)
    SYNC_PROP(part_launchers, mPartLaunchers)
    SYNC_PROP_TRIGGER(enable_events, mEnableEvents)
    SYNC_PROP_TRIGGER(disable_events, mDisableEvents)
    SYNC_PROP(enabled, mEnabled)
    SYNC_PROP_MODIFY(enabled_at_start, mEnabledAtStart, mEnabled = mEnabledAtStart)
    SYNC_PROP_TRIGGER(wait_for_events, mWaitForEvents)
    SYNC_PROP_SET(next_link, mNextLink.Ptr(), SetNextLink(_val.Obj<EventTrigger>()))
    SYNC_PROP(trigger_order, (int &)mTriggerOrder)
    SYNC_PROP(triggers_to_reset, mResetTriggers)
    SYNC_PROP(anim_trigger, (int &)mAnimTrigger)
    SYNC_PROP(anim_frame, mAnimFrame)
    SYNC_SUPERCLASS(RndAnimatable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS

EventTrigger::EventTrigger()
    : mAnims(this), mSpawnedTasks(this), mProxyCalls(this), mSounds(this), mShows(this),
      mResetTriggers(this), mHideDelays(this), mNextLink(this), mPartLaunchers(this),
      unkd0(false), mAnimTrigger(kTriggerAnimNone), mAnimFrame(0), mEnabled(true),
      mEnabledAtStart(true), mWaiting(false), mHidden(this), mShown(this),
      mTriggered(false), mTriggerOrder(kTriggerRandom), unk110(-1) {
    RegisterEvents();
}

void EventTrigger::Init() {
    REGISTER_OBJ_FACTORY(EventTrigger);
    DataRegisterFunc("cleanup_triggers", Cleanup);
}
