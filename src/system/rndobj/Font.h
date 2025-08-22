#pragma once
#include "rndobj/FontBase.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"

class KerningTable {
public:
    KerningTable();
    ~KerningTable();
    float Kerning(unsigned short, unsigned short);
    void GetKerning(std::vector<RndFontBase::KernInfo> &) const;
    void SetKerning(const std::vector<RndFontBase::KernInfo> &, RndFontBase *);
    void Save(BinStream &);

    MEM_OVERLOAD(KerningTable, 0x162);
};
