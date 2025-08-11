#pragma once
#include "utl/Str.h"

unsigned short WToLower(unsigned short);
unsigned short WToUpper(unsigned short);
int WStrniCmp(const unsigned short *, const unsigned short *, int);
unsigned int DecodeUTF8(unsigned short &, const char *);
unsigned int EncodeUTF8(String &, unsigned int);
void UTF8toASCIIs(char *, int, const char *, char);
void ASCIItoUTF8(char *, int, const char *);
int UTF8StrLen(const char *);
const char *UTF8strchr(const char *, unsigned short);
// void UTF8ToLower(unsigned short, char *);
// void UTF8ToUpper(unsigned short, char *);
void UTF8FilterString(char *, int, const char *, const char *, char);
void UTF8toWChar_t(wchar_t *wc, const char *c);

void ASCIItoWideVector(std::vector<unsigned short> &vec, const char *cc);
unsigned int WideVectorToUTF8(const std::vector<unsigned short> &vec, String &str);
void UTF8FilterKeyboardString(char *, int, const char *); // defined in os/PlatformMgr.cpp
                                                          // for some reason
String WideVectorToASCII(const std::vector<unsigned short> &);
const char *WideCharToChar(const unsigned short *us);
void UTF8RemoveSpaces(char *, int, const char *);
void UTF8toUTF16(unsigned short *us, const char *c);

void UTF8toWideVector(std::vector<unsigned short> &, const char *);
unsigned short *CharToWideChar(const char *);
