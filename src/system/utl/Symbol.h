#pragma once
#include <cstring>

extern const char* gNullStr;

#define STR_TO_SYM(str) *reinterpret_cast<Symbol *>(const_cast<char **>(&str))

class Symbol {
private:
    const char* mStr;
public:
    Symbol() : mStr(gNullStr) {}
    Symbol(const char *);
    Symbol(const Symbol &rhs) : mStr(rhs.mStr) {}

    const char* Str() const { return mStr; }
    bool operator<(const Symbol &s) const { return mStr < s.mStr; }
    bool Null() const { return mStr == gNullStr; }    
    bool operator==(const Symbol &s) const { return mStr == s.mStr; }
    bool operator!=(const Symbol &s) const { return mStr != s.mStr; }
    bool operator!=(const char *cc) const { return !(*this == cc); }
    bool operator==(const char *cc) const {
        if (cc)
            return strcmp(mStr, cc) == 0;
        else
            return Null();
    }

    static void PreInit(int, int);
    static void Init(void);
    static void Terminate(void);
};
