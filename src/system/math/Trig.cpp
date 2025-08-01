#include "math/Trig.h"
#include "obj/Data.h"
#include <cmath>

DataNode DataSin(DataArray *a) { return (float)sin(DegreesToRadians(a->Float(1))); }

void TrigInit() {}
