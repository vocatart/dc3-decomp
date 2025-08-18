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

bool UsingCD();
void SetUsingCD(bool);

DataArray *SystemConfig();
DataArray *SystemConfig(Symbol);
DataArray *SystemConfig(Symbol, Symbol);
DataArray *SystemConfig(Symbol, Symbol, Symbol);
DataArray *SystemConfig(Symbol, Symbol, Symbol, Symbol);
DataArray *SystemConfig(Symbol, Symbol, Symbol, Symbol, Symbol);

Symbol PlatformSymbol(Platform);
bool PlatformLittleEndian(Platform);
Platform ConsolePlatform();

Symbol SystemLanguage();
Symbol SystemLocale();
DataArray *SystemTitles();
Symbol GetSongTitlePronunciationLanguage();

int SystemExec(const char *);
int SystemMs();
void SystemPoll();

bool HongKongExceptionMet();
Symbol GetSystemLanguage(Symbol);

DataArray *ReadSystemConfig(const char *);
void StripEditorData();

DataArray *SupportedLanguages(bool);
bool IsSupportedLanguage(Symbol, bool);
void SetSystemLanguage(Symbol, bool);

void AppendStackTrace(FixedString &, void *);
void AppendThreadStackTrace(FixedString &, struct StackData *);

void LanguageInit();
void InitSystem(const char *);
void NormalizeSystemArgs();
void PreInitSystem(const char *);
void SetSystemArgs(const char *);
void SystemInit(const char *);
void SystemTerminate();
void SystemPreInit(const char *);
void SystemPreInit(const char *, const char *);

unsigned long ULSystemLocale();
unsigned long ULSystemLanguage();

typedef void DiscErrorCallbackFunc(void);

DiscErrorCallbackFunc *SetDiskErrorCallback(DiscErrorCallbackFunc *func);
DiscErrorCallbackFunc *GetDiskErrorCallback();

bool PlatformDebugBreak();
