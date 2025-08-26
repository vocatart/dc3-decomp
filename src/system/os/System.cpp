#include "os/System.h"
#include "HolmesClient.h"
#include "obj/Data.h"
#include "obj/DataFile.h"
#include "os/Debug.h"
#include "os/File.h"
#include "os/Platform.h"
#include "os/PlatformMgr.h"
#include "os/Timer.h"
#include "utl/Loader.h"
#include "utl/Locale.h"
#include "utl/Option.h"
#include <cstdarg>
#include <cstdio>

const char *gNullStr = "";

Symbol gSystemLanguage;
Symbol gSystemLocale;
DataArray *gSystemConfig;
DataArray *gSystemTitles;

int gUsingCD;
GfxMode gGfxMode;

int gSystemMs;
float gSystemFrac;
Timer gSystemTimer;
bool gNetUseTimedSleep;

std::vector<char *> TheSystemArgs;
const char *gHostFile;

int Hx_snprintf(char *c, unsigned int ui, char const *cc, ...) {
    std::va_list args;
    // va_start(args, cc);
    int ret = vsnprintf(c, ui, cc, args);
    if (ret < 0) {
        c[ui - 1] = '\0';
        return -1;
    }
    return ret;
}

GfxMode GetGfxMode() { return gGfxMode; }

Symbol PlatformSymbol(Platform pform) {
    static Symbol sym[] = { gNullStr, gNullStr, "xbox", "pc", "ps3", "wii", "3ds" };
    if (pform >= 0 && pform < 7) {
        return sym[pform];
    } else
        return gNullStr;
}

bool UsingCD() { return gUsingCD; }
void SetUsingCD(bool b) { gUsingCD = b; }

DataArray *SystemConfig() { return gSystemConfig; }

DataArray *SystemConfig(Symbol s) { return gSystemConfig->FindArray(s); }

DataArray *SystemConfig(Symbol s1, Symbol s2) {
    return gSystemConfig->FindArray(s1)->FindArray(s2);
}
DataArray *SystemConfig(Symbol s1, Symbol s2, Symbol s3) {
    return gSystemConfig->FindArray(s1)->FindArray(s2)->FindArray(s3);
}

DataArray *SystemConfig(Symbol s1, Symbol s2, Symbol s3, Symbol s4) {
    return gSystemConfig->FindArray(s1)->FindArray(s2)->FindArray(s3)->FindArray(s4);
}

DataArray *SystemConfig(Symbol s1, Symbol s2, Symbol s3, Symbol s4, Symbol s5) {
    return gSystemConfig->FindArray(s1)
        ->FindArray(s2)
        ->FindArray(s3)
        ->FindArray(s4)
        ->FindArray(s5);
}

Symbol SystemLanguage() { return gSystemLanguage; }
Symbol SystemLocale() { return gSystemLocale; }

DataArray *SystemTitles() { return gSystemTitles; }

Symbol GetSongTitlePronunciationLanguage() {
    Symbol lang = HongKongExceptionMet() ? "eng" : gSystemLanguage;
    static Symbol fre("fre");
    static Symbol frc("frc");
    static Symbol can("can");
    if (lang == fre && gSystemLocale == can) {
        lang = frc;
    }
    return lang;
}

int SystemExec(const char *args) {
    if (gUsingCD)
        return -1;
    else
        return HolmesClientSysExec(args);
}

namespace {
    void CheckForArchive() {
        gUsingCD = true;
        FileStat buffer;
        int ret = FileGetStat(
            MakeString("gen/main_%s.hdr", PlatformSymbol(TheLoadMgr.GetPlatform())),
            &buffer
        );
        gUsingCD &= ret;
    }
}

bool PlatformLittleEndian(Platform p) {
    MILO_ASSERT(p != kPlatformNone, 0x175);
    return p == kPlatformPC || p == kPlatform3DS || p == kPlatformNone;
}

Platform ConsolePlatform() { return kPlatformXBox; }

bool gReadingSystemConfig;

DataArray *ReadSystemConfig(const char *path) {
    gReadingSystemConfig = true;
    DataArray *config = DataReadFile(path, true);
    gReadingSystemConfig = false;
    return config;
}

void StripEditorData() {
    Symbol editor("editor");
    Symbol types("types");
    DataArray *objectsCfg = SystemConfig("objects");
    for (int i = 1; i < objectsCfg->Size(); i++) {
        DataArray *objectsArr = objectsCfg->Array(i);
        DataArray *objEditorArr = objectsArr->FindArray(editor, false);
        if (objEditorArr != 0)
            objEditorArr->Resize(1);
        DataArray *typesArr = objectsArr->FindArray(types, false);
        if (typesArr != 0) {
            for (int j = 1; j < typesArr->Size(); j++) {
                DataArray *typesEditorArr = typesArr->Array(j)->FindArray(editor, false);
                if (typesEditorArr != 0)
                    typesEditorArr->Resize(1);
            }
        }
    }
}

int SystemMs() {
    gSystemTimer.Restart();
    float lastMs = gSystemTimer.GetLastMs();
    int ms = gSystemFrac + lastMs;
    gSystemFrac = (gSystemFrac + lastMs) - ms;
    gSystemMs += ms;
    return gSystemMs;
}

void SystemPoll(bool b1) {
    static Timer *_t = AutoTimer ::GetTimer("system_poll");
    AutoTimer _at(_t, 50.0f, nullptr, nullptr);
    Timer::ClearSlowFrame();
    SystemMs();
    TheDebug.Poll();
    //   MemcardXbox::Poll(&TheMC);
    //   if (gUsingCD == 0) {
    //     HolmesClientPoll();
    //   }
    //   JoypadPoll();
    //   JoypadClientPoll();
    //   KeyboardPoll();
    //   ThreadCallPoll();
    //   FileCache::PollAll();
    //   LoadMgr::Poll(&TheLoadMgr);
    //   (**(*TheCacheMgr + 4))();
    //   (**(*TheNetCacheMgr + 0x58))();
    //   (**(*TheWebSvcMgr + 0x5c))();
    //   if (TheAppChild != 0x0) {
    //     AppChild::Poll(TheAppChild);
    //   }
    //   if (param_1) {
    //     TaskMgr::Poll(&TheTaskMgr);
    //   }
    //   PlatformMgr::Poll(&ThePlatformMgr);
    //   VirtualKeyboard::Poll(&TheVirtualKeyboard);
    //   (**(*TheContentMgr + 0x68))();
}

DataArray *SupportedLanguages(bool b) {
    static Symbol system("system");
    static Symbol language("language");
    static Symbol supported("supported");
    static Symbol cheat_supported("cheat_supported");
    return SystemConfig(system, language, b ? cheat_supported : supported)->Array(1);
}

bool IsSupportedLanguage(Symbol s, bool b) {
    DataArray *languages = SupportedLanguages(b);
    for (int i = 0; i < languages->Size(); i++) {
        if (languages->Sym(i) == s)
            return true;
    }
    return false;
}

void SetSystemLanguage(Symbol lang, bool b2) {
    if (!IsSupportedLanguage(lang, b2)) {
        static Symbol system("system");
        static Symbol language("language");
        static Symbol defaultSym("default");
        DataArray *arr = SystemConfig(system, language)->FindArray(defaultSym, false);
        if (arr) {
            Symbol arrLang = arr->Sym(0);
            if (IsSupportedLanguage(arrLang, b2)) {
                lang = arrLang;
            } else {
                MILO_NOTIFY(
                    "Both %s and the default language (%s) are not supported!\n",
                    lang,
                    arrLang
                );
            }
        } else {
            MILO_NOTIFY(
                "Language %s is not supported, and there is no default language found!\n",
                lang
            );
        }
    }
    if (!gSystemLanguage.Null() && lang != gSystemLanguage) {
        TheLocale.Terminate();
        gSystemLanguage = lang;
        TheLocale.Init();
    }
}

void SetGfxMode(GfxMode mode) {
    gGfxMode = mode;
    HolmesClientReInit();
    DataVariable("gfx_mode") = mode;
}

DataNode OnSystemLanguage(DataArray *) { return gSystemLanguage; }
DataNode OnSystemLocale(DataArray *) { return gSystemLocale; }
DataNode OnSystemExec(DataArray *a) { return SystemExec(a->Str(1)); }
DataNode OnUsingCD(DataArray *) { return UsingCD(); }
DataNode OnSupportedLanguages(DataArray *) { return SupportedLanguages(false); }
DataNode OnSystemMs(DataArray *) { return SystemMs(); }

DataNode OnSwitchSystemLanguage(DataArray *a) {
    DataArray *langs = SupportedLanguages(true);
    for (int i = 0; i < langs->Size(); i++) {
        Symbol cur = langs->Sym(i);
        if (gSystemLanguage == cur) {
            SetSystemLanguage(cur, true);
            break;
        }
    }
    return 0;
}

void LanguageInit() {
    if (ThePlatformMgr.GetRegion() == kRegionNone) {
        MILO_NOTIFY("LanguageInit called, but region has not been initialized");
    }
    DataArray *cfg = SystemConfig("system", "language");
    Symbol lang = GetSystemLanguage("eng");
    DataArray *remapArr = cfg->FindArray("remap", false);
    if (remapArr) {
        remapArr->FindData(lang, lang, false);
    }
    Symbol forceSym;
    if (cfg->FindData("force", forceSym, false)) {
        if (forceSym != "") {
            lang = forceSym;
        }
    }
    const char *str = OptionStr("lang", nullptr);
    if (str) {
        lang = str;
    }
    SetSystemLanguage(lang, false);
}
