#pragma once
#include "obj/Data.h"
#include "utl/MemMgr.h"
#include "math/Vec.h"

class Interpolator {
public:
    Interpolator() : mY(0, 0), mX(1, 1) {}
    virtual float Eval(float) const = 0;
    virtual float ClampEval(float f) const { return Eval(f); }
    virtual void Reset(const DataArray *) = 0;
    virtual ~Interpolator() {} // generic dtor

    MEM_OVERLOAD(Interpolator, 0x28);

protected:
    Vector2 mY; // 0x4
    Vector2 mX; // 0xc
};

class ATanInterpolator : public Interpolator {
private:
    float mSeverity; // 0x14
    float mSlope; // 0x18
    float mB; // 0x1c
    float mScale; // 0x20
    float mOffset; // 0x24
protected:
    void Sync();

public:
    ATanInterpolator(const char *, const char *);
    virtual ~ATanInterpolator() {}
    virtual float Eval(float) const;
    virtual void Reset(const DataArray *);

    void Reset(const Vector2 &, const Vector2 &, float);
};
