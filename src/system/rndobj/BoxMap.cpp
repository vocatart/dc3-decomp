#include "rndobj/BoxMap.h"

bool BoxMapLighting::CacheData(LightParams_Spot &spot) {
    if (spot.unk50 > 0 && spot.unk58 >= spot.unk54
        && (spot.mColor.red > 0.003921569f || spot.mColor.green > 0.003921569f
            || spot.mColor.blue > 0.003921569f)) {
        float f3 = (spot.unk54 * spot.unk50) / (spot.unk58 - spot.unk54);
        Vector3 v58;
        Scale(spot.unk0, f3, v58);
        Vector3 v4c;
        Subtract(spot.unk40, v58, v4c);
        float f1 = spot.unk58 / (spot.unk54 + f3);
        f1 *= f1;
        float f2 = 1.0f / (spot.unk54 * 2.0f);
        f1 = (1.0f - f1) / (f1 + 1.0f);
        spot.unk20 = v4c;
        spot.unk30 = f1;
        spot.unk34 = 1.0f / (1.0f - f1);
        spot.unk38 = f2;
        spot.unk3c = f3 * f2;
        return true;
    } else {
        mQueued_Spot.RemoveEntry();
        return false;
    }
}

BoxMapLighting::BoxMapLighting() { Clear(); }

void BoxMapLighting::Clear() {
    mQueued_Directional.Clear();
    mQueued_Point.Clear();
    mQueued_Spot.Clear();
}
