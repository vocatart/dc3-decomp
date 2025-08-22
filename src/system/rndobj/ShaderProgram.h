#pragma once
#include "rndobj/ShaderOptions.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"

class RndShaderBuffer {
public:
    virtual ~RndShaderBuffer() {}
    virtual void *Storage() = 0;
    virtual unsigned int Size() const = 0;

    MEM_OVERLOAD(RndShaderBuffer, 0x11);
};

class RndShaderProgram {
public:
    RndShaderProgram() {}
    virtual ~RndShaderProgram();
    // 7 pures
    virtual void pure1() = 0;
    virtual void pure2() = 0;
    virtual void pure3() = 0;
    virtual RndShaderBuffer *pure4(int) = 0;

    void LoadShaderBuffer(BinStream &, int, RndShaderBuffer *&);
    bool Cache(ShaderType, const ShaderOptions &, RndShaderBuffer *, RndShaderBuffer *);

    static unsigned long InitModTime();

protected:
    void CopyErrorShader(ShaderType, const ShaderOptions &);
    void SaveShaderBuffer(const char *, RndShaderBuffer &);
    void LoadShaderBuffer(const char *, RndShaderBuffer *&);
};
