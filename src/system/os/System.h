#pragma once
#include "obj/Data.h"
#include "os/Platform.h"
#include "utl/Symbol.h"

int Hx_snprintf(char *, unsigned int, char const *, ...);

extern const char *gNullStr;
extern std::vector<char *> TheSystemArgs;
extern const char *gHostFile;
extern bool gHostCached;

enum GfxMode {
    kOldGfx = 0,
    kNewGfx = 1,
};

void SetGfxMode(GfxMode);
GfxMode GetGfxMode();

DataArray *SystemConfig();
DataArray *SystemConfig(Symbol);
DataArray *SystemConfig(Symbol, Symbol);
DataArray *SystemConfig(Symbol, Symbol, Symbol);

Symbol PlatformSymbol(Platform);
bool PlatformLittleEndian(Platform);
Platform ConsolePlatform(); // { return kPlatformXBox; }

bool UsingCD();
void SetUsingCD(bool);

int SystemExec(const char *);
int SystemMs();

bool HongKongExceptionMet();
