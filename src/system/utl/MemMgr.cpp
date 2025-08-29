#include "utl/MemMgr.h"

const char *gStlAllocName = "StlAlloc";
bool gStlAllocNameLookup = false;

void operator delete(void *v) { MemFree(v, "unknown", 0, "unknown"); }
