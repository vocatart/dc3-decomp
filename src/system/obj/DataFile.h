#pragma once
#include "obj/Data.h"
#include "utl/BinStream.h"

DataArray *DataReadString(const char *);
DataArray *ReadCacheStream(BinStream &, const char *);
DataArray *ReadEmbeddedFile(const char *, bool);
DataArray *DataReadFile(const char *, bool);
DataArray *DataReadStream(BinStream *);
DataArray *ParseArray();
void DataWriteFile(const char *, const DataArray *, int);
DataArray *LoadDtz(const char *, int);

void BeginDataRead();
void FinishDataRead();
