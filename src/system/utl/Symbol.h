#pragma once

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

    static void PreInit(int, int);
    static void Init(void);
    static void Terminate(void);
};
