#pragma once
#include "utl/CRC.h"
#include <map>

template <class T>
class RefRes {
public:
    int unk0;
    T *unk4;
};

template <class T>
class ResMgr {
public:
    virtual ~ResMgr() {}
    virtual void OnReleaseResource(void *) = 0;
    virtual void Dump();

    void *Get(Hmx::CRC);
    void ReserveRes(Hmx::CRC, void *);

protected:
    std::map<Hmx::CRC, RefRes<T> > unk4; // 0x4
};
