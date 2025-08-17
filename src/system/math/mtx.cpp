#include "math/Mtx.h"

Hmx::Matrix2 Hmx::Matrix2::sID(Vector2(1, 0), Vector2(0, 1));
Hmx::Matrix3 Hmx::Matrix3::sID(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));
Hmx::Matrix4 Hmx::Matrix4::sID(
    Vector4(1, 0, 0, 0), Vector4(0, 1, 0, 0), Vector4(0, 0, 1, 0), Vector4(0, 0, 0, 1)
);

Transform Transform::sID(Hmx::Matrix3::ID(), Vector3(0, 0, 0));
