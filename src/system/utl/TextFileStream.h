#pragma once
#include "utl/FileStream.h"
#include "utl/TextStream.h"

class TextFileStream : public TextStream {
public:
    TextFileStream(const char *, bool);
    virtual ~TextFileStream() {}
    virtual void Print(const char *);

    FileStream mFile;
};
