#pragma once
#include "obj/Object.h"

enum DiskError {
    kNoDiskError,
    kDiskError,
    kWrongDisk,
    kFailedChecksum
};

enum PlatformRegion {
    kRegionNone,
    kRegionNA,
    kRegionEurope,
    kRegionJapan,
    kNumRegions
};

enum NotifyLocation {
    kNotify0,
    kNotify1,
    kNotify2
};

enum QuitType {
    kQuitNone,
    kQuitShutdown,
    kQuitRestart,
    kQuitMenu,
    kQuitDataManager
};

class PlatformMgr : public Hmx::Object {
public:
    PlatformMgr();
    PlatformRegion GetRegion() const;
};

extern PlatformMgr ThePlatformMgr;
