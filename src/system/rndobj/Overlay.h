#pragma once
#include "math/Color.h"
#include "obj/Data.h"
#include "os/Timer.h"
#include "utl/MemMgr.h"
#include "utl/TextStream.h"

class RndOverlay : public TextStream {
public:
    class Callback {
    public:
        Callback() {}
        virtual ~Callback() {}
        virtual float UpdateOverlay(RndOverlay *, float) {}
    };

    virtual void Print(const char *);

    MEM_OVERLOAD(RndOverlay, 0x13);

    int NumLines() const { return mLines.size(); }
    void Clear();
    void SetLines(int);
    void SetTimeout(float);
    String &CurrentLine();

    static void Init();
    static void Terminate();
    static void TogglePosition();
    static void DrawAll(bool);
    static RndOverlay *Find(Symbol, bool);

private:
    RndOverlay(const DataArray *);

    float Draw(float);

    static bool sTopAligned;
    static std::list<RndOverlay *> sOverlays;

    const char *mName; // 0x4
    bool mShowing; // 0x8
    std::list<String> mLines; // 0xc
    std::list<String>::iterator mLine; // 0x14
    Hmx::Color mBackColor; // 0x18
    Hmx::Color mTextColor; // 0x28
    int mCursorChar; // 0x38
    Callback *mCallback; // 0x3c
    Timer mTimer; // 0x40
    float mTimeout; // 0x70
    bool mModal; // 0x74
    int mDumpCount; // 0x78
};
