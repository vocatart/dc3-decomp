#include "Rot.h"
#include "math/Mtx.h"
#include "math/Vec.h"
#include <cmath>

TextStream &operator<<(TextStream &ts, const Hmx::Quat &v) {
    ts << "(x:" << v.x << " y:" << v.y << " z:" << v.z << " w:" << v.w << ")";
    return ts;
}

TextStream &operator<<(TextStream &ts, const Vector3 &v) {
    ts << "(x:" << v.x << " y:" << v.y << " z:" << v.z << ")";
    return ts;
}

TextStream &operator<<(TextStream &ts, const Vector2 &v) {
    ts << "(x:" << v.x << " y:" << v.y << ")";
    return ts;
}

TextStream &operator<<(TextStream &ts, const Hmx::Matrix3 &m) {
    ts << "\n\t" << m.x << "\n\t" << m.y << "\n\t" << m.z;
    return ts;
}

TextStream &operator<<(TextStream &ts, const Transform &t) {
    ts << t.m << "\n\t" << t.v;
    return ts;
}

Hmx::Matrix4 &Hmx::Matrix4::Zero() {
    memset(this, 0, sizeof(*this));
    return *this;
}

void Hmx::Quat::Set(const Vector3 &v, float f) {
    float scale = Sine(f / 2);
    w = Cosine(f / 2);
    x = v.x * scale;
    y = v.y * scale;
    z = v.z * scale;
}

float GetXAngle(const Hmx::Matrix3 &m) { return atan2(m.y.z, m.y.y); }
float GetYAngle(const Hmx::Matrix3 &m) { return atan2(-m.x.z, m.z.z); }
float GetZAngle(const Hmx::Matrix3 &m) {
    float res = atan2(m.y.x, m.y.y);
    return -res;
}

void Interp(const Hmx::Quat &q1, const Hmx::Quat &q2, float r, Hmx::Quat &qres) {
    Nlerp(q1, q2, r, qres);
}

void Interp(const Hmx::Matrix3 &m1, const Hmx::Matrix3 &m2, float r, Hmx::Matrix3 &res) {
    Hmx::Quat q40(m1);
    Hmx::Quat q50(m2);
    Hmx::Quat q60;
    Nlerp(q40, q50, r, q60);
    MakeRotMatrix(q60, res);
}
