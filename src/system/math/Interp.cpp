#include "math/Interp.h"
#include "math/Vec.h"
#include "obj/Data.h"
#include <cmath>

float ATanInterpolator::Eval(float f1) const {
    float tanned = atan(mSlope * f1 + mB);
    return mScale * tanned + mOffset;
}

ATanInterpolator::ATanInterpolator(const char *, const char *)
    : Interpolator(), mSeverity(2) {
    Sync();
}

void ATanInterpolator::Reset(const Vector2 &y, const Vector2 &x, float sev) {
    mY = y;
    mX = x;
    mSeverity = sev;
    Sync();
}

void ATanInterpolator::Reset(const DataArray *a) {
    float sev = a->Size() > 5 ? a->Float(5) : 10.0f;
    float x2 = a->Float(2);
    float y2 = a->Float(4);
    float x1 = a->Float(1);
    float y1 = a->Float(3);
    mSeverity = sev;
    mY.Set(y1, x1);
    mX.Set(y2, x2);
    Sync();
}
