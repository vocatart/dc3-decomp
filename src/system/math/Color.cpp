#include "math/Color.h"
#include "utl/TextStream.h"

TextStream &operator<<(TextStream &ts, const Hmx::Color &color) {
    ts << "(r:" << color.red << " g:" << color.green << " b:" << color.blue
       << " a:" << color.alpha << ")";
    return ts;
}

void MakeColor(float f1, float f2, float f3, Hmx::Color &color) {
    if (f2 == 0) {
        color.Set(f3, f3, f3);
        return;
    }
    // more...
}

void MakeHSL(const Hmx::Color &color, float &f1, float &f2, float &f3) {
    float maxCol = Max(color.red, color.green, color.blue);
    float minCol = Min(color.red, color.green, color.blue);
    f3 = (maxCol + minCol) / 2.0f;
    if (maxCol == minCol) {
        f1 = 0;
        f2 = 0;
    } else {
        float deltaCol = maxCol - minCol;
        if (f3 < 0.5f)
            f2 = deltaCol / (minCol + maxCol);
        else
            f2 = deltaCol / ((2.0f - maxCol) - minCol);
        if (color.red == maxCol) {
            f1 = (color.green - color.blue) / deltaCol;
        } else if (color.green == maxCol) {
            f1 = (color.blue - color.red) / deltaCol + 2.0f;
        } else {
            f1 = (color.red - color.green) / deltaCol + 4.0f;
        }
        f1 /= 6.0f;
        if (f1 < 0.0f)
            f1 += 1.0f;
    }
}
