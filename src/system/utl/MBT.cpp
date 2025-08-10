#include "utl/MBT.h"
#include "os/Debug.h"
#include <cmath>

const char *TickFormat(int tick, const MeasureMap &map) {
    int i1, i2, i3;
    if (tick >= 0) {
        map.TickToMeasureBeatTick(tick, i1, i2, i3);
        return MakeString("%d:%d:%03d", i1 + 1, i2 + 1, i3);
    } else
        return "negative tick";
}

const char *FormatTimeMSH(float f) {
    return MakeString(
        "%d:%02d.%02d",
        (int)(f / 60000.0f),
        (int)(std::fmod(f, 60000.0) / 1000.0),
        (int)(std::fmod(f, 1000.0) / 10.0)
    );
}
