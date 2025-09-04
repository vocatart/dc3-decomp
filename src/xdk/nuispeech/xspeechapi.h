#pragma once
#include "../win_types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum _NUI_SPEECH_LANGUAGE {
    NUI_SPEECH_LANGUAGE_EN_US = 0x0000,
    NUI_SPEECH_LANGUAGE_FR_CA = 0x0001,
    NUI_SPEECH_LANGUAGE_EN_GB = 0x0002,
    NUI_SPEECH_LANGUAGE_ES_MX = 0x0003,
    NUI_SPEECH_LANGUAGE_JA_JP = 0x0004,
    NUI_SPEECH_LANGUAGE_FR_FR = 0x0005,
    NUI_SPEECH_LANGUAGE_ES_ES = 0x0006,
    NUI_SPEECH_LANGUAGE_DE_DE = 0x0007,
    NUI_SPEECH_LANGUAGE_IT_IT = 0x0008,
};
typedef _NUI_SPEECH_LANGUAGE NUI_SPEECH_LANGUAGE;

enum NUI_SPEECH_MICTYPE {
    NUI_SPEECH_KINECT = 0x0000,
    NUI_SPEECH_WAVE = 0x0001,
};

struct _NUI_SPEECH_INIT_PROPERTIES { /* Size=0x8 */
    /* 0x0000 */ _NUI_SPEECH_LANGUAGE Language;
    /* 0x0004 */ NUI_SPEECH_MICTYPE MicrophoneType;
};
typedef _NUI_SPEECH_INIT_PROPERTIES NUI_SPEECH_INIT_PROPERTIES;

struct _NUI_SPEECH_GRAMMAR { /* Size=0x8 */
    /* 0x0000 */ unsigned long ulId;
    /* 0x0004 */ void *hGrammar;
};
typedef _NUI_SPEECH_GRAMMAR NUI_SPEECH_GRAMMAR;

HRESULT NuiSpeechCreateGrammar(unsigned long, NUI_SPEECH_GRAMMAR *);
HRESULT NuiSpeechEnable(NUI_SPEECH_INIT_PROPERTIES *, DWORD);
HRESULT NuiSpeechDisable();
HRESULT NuiSpeechSetEventInterest(unsigned long);
HRESULT NuiSpeechStartRecognition();
HRESULT NuiSpeechStopRecognition();

#ifdef __cplusplus
}
#endif
