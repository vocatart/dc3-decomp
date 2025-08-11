#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum XC_LANGUAGE {
    XC_LANGUAGE_UNKNOWN = 0,
    XC_LANGUAGE_ENGLISH = 1,
    XC_LANGUAGE_JAPANESE = 2,
    XC_LANGUAGE_GERMAN = 3,
    XC_LANGUAGE_FRENCH = 4,
    XC_LANGUAGE_SPANISH = 5,
    XC_LANGUAGE_ITALIAN = 6,
    XC_LANGUAGE_KOREAN = 7,
    XC_LANGUAGE_TCHINESE = 8,
    XC_LANGUAGE_PORTUGUESE = 9,
    XC_LANGUAGE_SCHINESE = 10,
    XC_LANGUAGE_POLISH = 11,
    XC_LANGUAGE_RUSSIAN = 12
};

unsigned long XTLGetLanguage();

#ifdef __cplusplus
}
#endif
