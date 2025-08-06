#pragma once
#include "math/Vec.h"
#include "math/Trig.h"
#include "utl/BinStream.h"

namespace Hmx {
    class Matrix3 {
    public:
        Vector3 x;
        Vector3 y;
        Vector3 z;

        // all of these are weak
        Matrix3() {}

        Matrix3(const Matrix3 &mtx) {
            x = mtx.x;
            y = mtx.y;
            z = mtx.z;
        }

        Matrix3(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
            : x(v1), y(v2), z(v3) {}

        // clang-format off
        Matrix3(
            float f1, float f2, float f3,
            float f4, float f5, float f6,
            float f7, float f8, float f9
        )
            : x(f1, f2, f3), y(f4, f5, f6), z(f7, f8, f9) {}

        void Set(
            float f1, float f2, float f3,
            float f4, float f5, float f6,
            float f7, float f8, float f9
        ) {
            x.Set(f1, f2, f3);
            y.Set(f4, f5, f6);
            z.Set(f7, f8, f9);
        }
        // clang-format on
        void Set(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) {
            x = v1;
            y = v2;
            z = v3;
        }
        void Zero() {
            x.Zero();
            y.Zero();
            z.Zero();
        }
        void RotateAboutZ(float angle) {
            float c = Cosine(angle);
            float s = Sine(angle);
            Set(c, -s, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 1.0f);
        }
        void RotateAboutY(float angle) {
            float c = Cosine(angle);
            float s = Sine(angle);
            Set(c, 0.0f, -s, 0.0f, 1.0f, 0.0f, s, 0.0f, c);
        }
        void RotateAboutX(float angle) {
            float c = Cosine(angle);
            float s = Sine(angle);
            Set(1.0f, 0.0f, 0.0f, 0.0f, c, s, 0.0f, -s, c);
        }
        void Identity() {
            x.Set(1.0f, 0.0f, 0.0f);
            y.Set(0.0f, 1.0f, 0.0f);
            z.Set(0.0f, 0.0f, 1.0f);
        }
        Matrix3 &operator=(const Matrix3 &mtx);
        Vector3 &operator[](int i) { return *(&x + i); }

        bool operator==(const Matrix3 &mtx) const {
            return x == mtx.x && y == mtx.y && z == mtx.z;
        }

        bool operator!=(const Matrix3 &mtx) const {
            return x != mtx.x || y != mtx.y || z != mtx.z;
        }
    };

    class Quat {
    public:
        Quat() {}
        Quat(float f1, float f2, float f3, float f4) : x(f1), y(f2), z(f3), w(f4) {}
        Quat(const Matrix3 &m) { Set(m); }
        Quat(const Vector3 &v) { Set(v); }
        Quat(const Vector3 &, float);

        void Reset() {
            x = y = z = 0.0f;
            w = 1.0f;
        }
        void Zero() { w = x = y = z = 0.0f; }
        void Set(const Matrix3 &);
        void Set(const Vector3 &);
        void Set(const Vector3 &, float);
        void Set(float f1, float f2, float f3, float f4) {
            x = f1;
            y = f2;
            z = f3;
            w = f4;
        }

        bool operator!=(const Quat &q) const {
            return x != q.x || y != q.y || z != q.z || w != q.w;
        }

        float x;
        float y;
        float z;
        float w;
    };
}

inline BinStream &operator<<(BinStream &bs, const Hmx::Matrix3 &mtx) {
    bs << mtx.x << mtx.y << mtx.z;
    return bs;
}

inline BinStream &operator>>(BinStream &bs, Hmx::Matrix3 &mtx) {
    bs >> mtx.x >> mtx.y >> mtx.z;
    return bs;
}

inline BinStream &operator<<(BinStream &bs, const Hmx::Quat &q) {
    bs << q.x << q.y << q.z << q.w;
    return bs;
}

inline BinStream &operator>>(BinStream &bs, Hmx::Quat &q) {
    bs >> q.x >> q.y >> q.z >> q.w;
    return bs;
}

class Transform {
public:
    class Hmx::Matrix3 m;
    class Vector3 v;

    // all of these are weak
    Transform() {}

    Transform(const Hmx::Matrix3 &mtx, const Vector3 &vec) : m(mtx), v(vec) {}

    Transform(const Transform &tf);
    Transform &operator=(const Transform &tf);

    void Reset() {
        m.Identity();
        v.Zero();
    }

    void Set(const Hmx::Matrix3 &mtx, const Vector3 &vec) {
        m = mtx;
        v = vec;
    }

    void LookAt(const Vector3 &, const Vector3 &);
    void Zero() {
        m.Zero();
        v.Zero();
    }

    bool operator==(const Transform &tf) const { return m == tf.m && v == tf.v; }
};

inline BinStream &operator<<(BinStream &bs, const Transform &tf) {
    bs << tf.m << tf.v;
    return bs;
}

inline BinStream &operator>>(BinStream &bs, Transform &tf) {
    bs >> tf.m >> tf.v;
    return bs;
}
