#pragma once
#include "math/Vec.h"

class DoubleExponentialSmoother {
public:
    DoubleExponentialSmoother();
    DoubleExponentialSmoother(float, float, float);
    void Smooth(float, float);

protected:
    float unk0, unk4, unk8, unkc, unk10;
};

class Vector2DESmoother {
public:
    void SetSmoothParameters(float, float);
    void Smooth(Vector2, float, bool);
    Vector2 Value() const;
    void ForceValue(Vector2);

protected:
    DoubleExponentialSmoother mSmootherX;
    DoubleExponentialSmoother mSmootherY;
};

class Vector3DESmoother {
public:
    Vector3DESmoother() {}
    Vector3DESmoother(Vector3, float, float);
    void SetSmoothParameters(float, float);
    void Smooth(Vector3, float, bool);
    Vector3 Value() const;
    void ForceValue(Vector3);

protected:
    DoubleExponentialSmoother mSmootherX;
    DoubleExponentialSmoother mSmootherY;
    DoubleExponentialSmoother mSmootherZ;
};
