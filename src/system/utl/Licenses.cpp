#include "Licenses.h"

Licenses *Licenses::sFront = nullptr;
int Licenses::sInited = 0;

CDECL Licenses::Licenses(const char *cc, Licenses::Requirement req) {
    unk_0x4 = cc;
    unk_0x0 = req;
    Licenses *next;
    int magic = 0xFEEDBACC;
    if (sInited != magic) {
        sInited = magic;
        next = nullptr;
    } else {
        next = sFront;
    }
    mNext = next;
    sFront = this;
}

void Licenses::PrintAll(void) {}
