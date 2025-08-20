#pragma once
#include "rndobj/Trans.h"
#include "math/Vec.h"
#include "math/Color.h"
#include <vector>

void SetLocalScale(RndTransformable *, const Vector3 &);

DataNode GetNormalMapTextures(ObjectDir *);

void ResetColors(std::vector<Hmx::Color> &colors, int newNumColors);
