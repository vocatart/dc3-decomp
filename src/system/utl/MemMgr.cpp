#include "utl/MemMgr.h"

void operator delete(void *v) { MemFree(v, "unknown", 0, "unknown"); }
