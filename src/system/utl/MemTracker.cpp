#include "utl/MemTracker.h"
#include "AllocInfo.h"
#include "MemMgr.h"
#include "MemTrack.h"
#include "obj/DataFunc.h"
#include "os/Debug.h"
#include "utl/KeylessHash.h"

MemTracker::MemTracker(int x, int y)
    : mHashMem(nullptr), mHashTable(nullptr), unk8(0), unk1817c(0),
      unk18180(DebugHeapAlloc(y * 4)), unk18184(unk18180),
      unk18188((char *)unk18180 + y * 4), unk1818c(0), unk18190(0), unk18194(x) {
    mHashMem = DebugHeapAlloc(y * 8);
    MILO_ASSERT(mHashMem, 0x4E);
    mHashTable = new KeylessHash<void *, AllocInfo *>(
        x * 2, (AllocInfo *)0, (AllocInfo *)-1, (AllocInfo **)mHashMem
    );
    unk18198 = _GetFreeSystemMemory();
    unk1819c = _GetFreePhysicalMemory();
    DataRegisterFunc("spit_alloc_info", SpitAllocInfo);
    DataRegisterFunc("sai", SpitAllocInfo);
}
