#pragma once
#include "math/Mtx.h"
#include "math/Vec.h"

#define PI 3.1415927f
#define RAD2DEG 57.29578f
#define DEG2RAD 0.01745329238474369049f

float GetXAngle(const Hmx::Matrix3 &);
float GetYAngle(const Hmx::Matrix3 &);
float GetZAngle(const Hmx::Matrix3 &);

void MakeEuler(const Hmx::Matrix3 &, Vector3 &);
void MakeEulerScale(const Hmx::Matrix3 &, Vector3 &, Vector3 &);
void MakeScale(const Hmx::Matrix3 &, Vector3 &);
void MakeRotMatrix(const Vector3 &, const Hmx::Matrix3 &, bool);
