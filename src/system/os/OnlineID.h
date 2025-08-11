#pragma once
#include "types.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"
#include "utl/Str.h"

// size 0x18
class OnlineID {
private:
    friend BinStream &operator<<(BinStream &, const OnlineID &);

    u64 mXUID; // 0x0
    String mPlayerName; // 0x8
    bool mValid; // 0x10
public:
    OnlineID();
    OnlineID(const u64 &);
    void Clear();
    void SetXUID(const u64 &);
    void SetPlayerName(const char *);
    u64 GetXUID() const;
    const char *ToString() const;

    MEM_OVERLOAD(OnlineID, 0x1E)
};

// BinStream &operator>>(BinStream &, OnlineID &);
