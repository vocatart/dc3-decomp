#include "gesture/SpeechMgr.h"
#include "SpeechMgr.h"
#include "obj/Data.h"
#include "obj/Dir.h"
#include "os/Debug.h"
#include "os/File.h"
#include "rndobj/Overlay.h"
#include "utl/MemMgr.h"
#include "xdk/nuispeech/xspeechapi.h"

SpeechMgr *TheSpeechMgr;

SpeechMgr::SpeechMgr(const DataArray *a)
    : mEnabled(0), mRecognizing(0), unk3e(0),
      mOverlay(RndOverlay::Find("speech_mgr", true)) {
    TheSpeechMgr = this;
    SetName("speech_mgr", ObjectDir::Main());
    mSpeechSupported = GetSpeechLanguage(mLanguage);
    mSpeechConfThresh = DetermineSpeechConfidenceThreshold();
    mOverlay->SetLines(16);
    unk44 = -1;
}

SpeechMgr::~SpeechMgr() {
    TheSpeechMgr = nullptr;
    DisableAndUnloadGrammars();
}

bool SpeechMgr::IsRecognizing() const { return mRecognizing; }

unsigned long gGrammarID;

void SpeechMgr::CreateGrammar(Symbol name) {
    for (int i = 0; i < mGrammars.size(); i++) {
        MILO_ASSERT(name != mGrammars[i].mName, 0x3A2);
    }
    Grammar grammar;
    grammar.unk14 = 0;
    grammar.mName = name;
    grammar.unk4 = "";
    HRESULT res = NuiSpeechCreateGrammar(gGrammarID++, &grammar.mGrammar);
    if (res < 0) {
        MILO_FAIL("NuiSpeechCreateGrammar failed with error 0x%08x", res);
    }
    mGrammars.push_back(grammar);
}

void SpeechMgr::EnableAndLoadGrammars(const DataArray *cfg) {
    if (!mEnabled) {
        Enable(false);
        if (mEnabled) {
            DataArray *grammarArr = cfg->FindArray("grammars");
            for (int i = 1; i < grammarArr->Size(); i++) {
                DataArray *curArr = grammarArr->Array(i);
                Grammar grammar;
                grammar.mName = curArr->Sym(0);
                String file =
                    String("grammar/") + GetSpeechLanguageDir() + "/" + curArr->Str(1);
                if (FileExists(file.c_str(), 0x10000, &grammar.unk4)
                    && grammar.FinishLoad(this)) {
                    mGrammars.push_back(grammar);
                    SetGrammarState(grammar.mName, false);
                }
            }
        }
        SpeechEnableMsg msg(mEnabled);
        Export(msg, false);
    }
}

static bool gDisableGrammars;

void SpeechMgr::DisableAndUnloadGrammars() {
    static SpeechEnableMsg speechEnableMsg(gDisableGrammars);
    Export(speechEnableMsg, false);
    for (std::vector<Grammar>::iterator it = mGrammars.begin(); it != mGrammars.end();
         ++it) {
        it->Unload();
    }
    mGrammars.clear();
    if (mEnabled) {
        Disable();
    }
}

void SpeechMgr::Enable(bool wave) {
    if (mSpeechSupported) {
        MILO_ASSERT(mEnabled == false, 0x13F);
        NUI_SPEECH_INIT_PROPERTIES props;
        props.MicrophoneType = (NUI_SPEECH_MICTYPE)wave;
        props.Language = mLanguage;
        MemForceNewOperatorAlign(16);
        HRESULT enableRes = NuiSpeechEnable(&props, 3);
        MemForceNewOperatorAlign(0);
        if (enableRes != 0) {
            if (enableRes == 0x80070057) {
                MILO_FAIL("NuiSpeechEnable failed E_INVALIDARG");
            } else if (enableRes == 0x8301000D) {
                MILO_FAIL("NuiSpeechEnable failed E_NUI_DATABASE_NOT_FOUND");
            } else if (enableRes == 0x8301000E) {
                MILO_FAIL("NuiSpeechEnable failed E_NUI_DATABASE_VERSION_MISMATCH");
            } else {
                MILO_LOG(
                    "NuiSpeechEnable failed with error 0x%x (no Kinect?)\n", enableRes
                );
            }
        } else {
            mEnabled = true;
            HRESULT interestRes = NuiSpeechSetEventInterest(0x40);
            if (interestRes < 0) {
                MILO_FAIL(
                    "NuiSpeechSetEventInterest failed with error 0x%08x", interestRes
                );
            }
        }
    }
}

void SpeechMgr::Disable() {
    if (mEnabled) {
        HRESULT stopRes = NuiSpeechStopRecognition();
        if (stopRes < 0) {
            MILO_FAIL("NuiSpeechStopRecognition failed with error 0x%08x", stopRes);
        }
        HRESULT disableRes = NuiSpeechDisable();
        if (disableRes < 0) {
            MILO_FAIL("NuiSpeechDisable failed with error 0x%08x", disableRes);
        }
        mEnabled = false;
    }
}

void SpeechMgr::InitGrammars(const DataArray *a) {
    if (mSpeechSupported)
        EnableAndLoadGrammars(a);
}

void SpeechMgr::LoadGrammar(Symbol name, const char *c, bool b) {
    MILO_ASSERT(std::find(mGrammars.begin(), mGrammars.end(), name) == mGrammars.end(), 0x263);
    MILO_ASSERT(mEnabled, 0x264);
    Grammar grammar;
    grammar.unk14 = 0;
    grammar.mName = name;
    if (FileExists(c, 0x10000, &grammar.unk4) && grammar.FinishLoad(this)) {
        mGrammars.push_back(grammar);
    }
}
