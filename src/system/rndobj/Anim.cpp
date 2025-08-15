#include "rndobj/Anim.h"
#include "math/Easing.h"
#include "obj/ObjMacros.h"
#include "obj/Msg.h"
#include "obj/ObjPtr_p.h"
#include "obj/ObjRef.h"
#include "obj/Task.h"
#include "os/Debug.h"
#include "utl/BinStream.h"

static TaskUnits gRateUnits[6] = { kTaskSeconds, kTaskBeats,           kTaskUISeconds,
                                   kTaskBeats,   kTaskTutorialSeconds, kTaskBeats };
static float gRateFpu[6] = { 30.0f, 480.0f, 30.0f, 1.0f, 30.0f, 15.0f };

TaskUnits RndAnimatable::RateToTaskUnits(Rate myRate) { return gRateUnits[myRate]; }
RndAnimatable::RndAnimatable() : mFrame(0.0f), mRate(k30_fps) {}
TaskUnits RndAnimatable::Units() const { return gRateUnits[mRate]; }
float RndAnimatable::FramesPerUnit() { return gRateFpu[mRate]; }

bool RndAnimatable::ConvertFrames(float &f) {
    f /= FramesPerUnit();
    return (Units() != kTaskBeats);
}

void RndAnimatable::SetFrame(float frame, float blend) {
    if (mFrame != frame) {
        static Symbol frameSym("frame");
        mFrame = frame;
        BroadcastPropertyChange(frameSym);
    }
}

BEGIN_COPYS(RndAnimatable)
    CREATE_COPY(RndAnimatable)
    BEGIN_COPYING_MEMBERS
        COPY_MEMBER(mFrame)
        COPY_MEMBER(mRate)
    END_COPYING_MEMBERS
END_COPYS

void RndAnimatable::Save(BinStream &bs) {
    bs << 4;
    bs << mFrame << mRate;
}

bool RndAnimatable::IsAnimating() {
    for (ObjRef::iterator it = mRefs.begin(); it != mRefs.end(); ++it) {
        if (dynamic_cast<AnimTask *>(it->RefOwner()))
            return true;
    }
    return false;
}

void RndAnimatable::StopAnimation() {
    for (ObjRef::iterator it = mRefs.begin(); it != mRefs.end();) {
        AnimTask *task = dynamic_cast<AnimTask *>(it->RefOwner());
        if (task) {
            delete task;
            it = mRefs.begin();
        } else
            ++it;
    }
}

// float AnimTask::TimeUntilEnd() {
//     float time;
//     if (mScale > 0.0f) {
//         float fpu = mAnim->FramesPerUnit();
//         time = (mMax - mAnim->GetFrame()) / fpu;
//     } else {
//         float fpu = mAnim->FramesPerUnit();
//         time = (mAnim->GetFrame() - mMin) / fpu;
//     }
//     return time;
// }

void RndAnimatable::FireFlowLabel(Symbol s) {
    if (!s.Null()) {
        for (ObjRef::iterator it = mRefs.begin(); it != mRefs.end(); ++it) {
            Hmx::Object *owner = it->RefOwner();
            if (owner && owner->ClassName() == "AnimTask") {
                AnimTask *task = static_cast<AnimTask *>(owner);
                if (task->AnimTarget()) {
                    owner->Handle(Message("on_anim_event", s), false);
                    break;
                }
            }
        }
        static Symbol flow_label_fired("flow_label_fired");
        Message msg(flow_label_fired, s.Str());
        Export(msg, true);
    }
}

DataNode RndAnimatable::OnConvertFrames(DataArray *arr) {
    float f = arr->Float(2);
    bool conv = ConvertFrames(f);
    *arr->Var(2) = f;
    return conv;
}

BEGIN_PROPSYNCS(RndAnimatable)
    SYNC_PROP(rate, (int &)mRate);
    SYNC_PROP_SET(frame, mFrame, SetFrame(_val.Float(), 1.0f))
    SYNC_PROP_SET(start_frame, StartFrame(), )
    SYNC_PROP_SET(end_frame, EndFrame(), )
END_PROPSYNCS

AnimTask::~AnimTask() { TheTaskMgr.QueueTaskDelete(mBlendTask); }

AnimTask::AnimTask(
    RndAnimatable *anim,
    float start,
    float end,
    float fpu,
    bool loop,
    float blend,
    Hmx::Object *,
    EaseType,
    float,
    bool
)
    : mAnim(this, nullptr), unk40(this, nullptr), mAnimTarget(this, nullptr),
      mBlendTask(this, nullptr), mBlendPeriod(blend), mLoop(loop) {
    MILO_ASSERT(anim, 0x1DF);
    mMin = Min(start, end);
    mMax = Max(start, end);

    Hmx::Object *target = anim->AnimTarget();
    if (target) {
        for (ObjRef::iterator it = target->Refs().begin(); it != target->Refs().end();
             ++it) {
            Hmx::Object *owner = it->RefOwner();
            if (owner && owner->ClassName() == StaticClassName()) {
                AnimTask *task = static_cast<AnimTask *>(owner);
                mBlendTask = task;
                MILO_ASSERT(mBlendTask != this, 0x231);
                break;
            }
        }
    }
    if (mBlendPeriod && mBlendTask) {
        mBlendTask->mBlending = true;
    }
    mAnim = anim;
    mAnimTarget = anim->AnimTarget();
}

bool AnimTask::Replace(ObjRef *ref, Hmx::Object *o) {
    if (ref == &mAnim) {
        RndAnimatable *myAnim = Anim();
        if (!mAnim.SetObj(o)) {
            if (mBlendTask && mBlendTask->Anim() == myAnim) {
                mBlendTask = nullptr;
            }
            Hmx::Object::Replace(ref, o);
            TheTaskMgr.QueueTaskDelete(this);
        }
        return true;
    } else
        return Hmx::Object::Replace(ref, o);
}

Task *RndAnimatable::Animate(
    float blend, bool wait, float delay, Hmx::Object *o, EaseType e, float f4, bool b5
) {
    AnimTask *task = new AnimTask(
        this, StartFrame(), EndFrame(), FramesPerUnit(), Loop(), blend, o, e, f4, b5
    );
    ObjPtr<AnimTask> taskPtr(nullptr, task);
    if (wait && task->BlendTask()) {
        delay += task->BlendTask()->TimeUntilEnd();
    }
    if (delay == 0) {
        SetFrame(StartFrame(), 1);
    }
    TheTaskMgr.Start(taskPtr, Units(), delay);
    return taskPtr;
}
