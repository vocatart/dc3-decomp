#include "math/DoubleExponentialSmoother.h"

DoubleExponentialSmoother::DoubleExponentialSmoother()
    : unk0(0), unk4(0), unk8(0), unkc(0), unk10(0) {}

DoubleExponentialSmoother::DoubleExponentialSmoother(float f1, float f2, float f3)
    : unk0(f1), unk4(f1), unk8(0), unkc(f2), unk10(f3) {}
