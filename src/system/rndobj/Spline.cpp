#include "rndobj/Spline.h"
#include "obj/Object.h"
#include "math/Rot.h"

RndSpline::CtrlPoint::CtrlPoint()
    : pos(Vector3::ZeroVec()), roll(0), unk14(1), unk15(1), unk18(Vector4::ZeroVec()),
      unk28(Vector4::ZeroVec()), unk38(Vector4::ZeroVec()), unk48(Vector4::ZeroVec()) {}

RndSpline::RndSpline()
    : mManual(false), mPulseLength(10), mPulseAmplitude(10), mStartCtrlPoint(-1),
      mEndCtrlPoint(-1), mYOffset(0), mYPerCtrlPoint(10), unk144(0), unk145(0), unk146(0),
      unk148(-1000), unk14c(0) {}

BEGIN_CUSTOM_PROPSYNC(RndSpline::CtrlPoint)
    SYNC_PROP(pos, o.pos)
    SYNC_PROP_SET(roll, o.roll * RAD2DEG, o.roll = _val.Float() * DEG2RAD)
END_CUSTOM_PROPSYNC

BEGIN_PROPSYNCS(RndSpline)
    SYNC_PROP_MODIFY(ctrl_points, mCtrlPoints, SyncPristineCtrlPoints())
    SYNC_PROP(manual, mManual)
    SYNC_PROP(pulse_length, mPulseLength)
    SYNC_PROP(pulse_amplitude, mPulseAmplitude)
    SYNC_PROP_SET(start_ctrl_point, mStartCtrlPoint, SetStartCtrlPoint(_val.Int()))
    SYNC_PROP_SET(end_ctrl_point, mEndCtrlPoint, SetEndCtrlPoint(_val.Int()))
    SYNC_PROP_SET(y_offset, mYOffset, mYOffset = _val.Float())
    SYNC_PROP_SET(
        y_per_ctrl_point, mYPerCtrlPoint, mYPerCtrlPoint = Min(0.1f, _val.Float())
    )
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
