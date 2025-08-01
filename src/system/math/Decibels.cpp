#include "os/Debug.h"
#include "math/Decibels.h"
#include <cmath>

float DbToRatio(float db) {
    if (db <= -96.0f)
        return 0.0f;
    else
        return pow(10.0f, db / 20.0f);
}

float RatioToDb(float ratio) {
    MILO_ASSERT(ratio >= 0.0f, 0x1B);
    if (ratio <= 0.0f)
        return -96.0f;
    else
        return log10(ratio) * 20.0f;
}
