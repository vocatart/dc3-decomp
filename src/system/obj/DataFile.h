#pragma once
#include "obj/Data.h"
#include "utl/BinStream.h"
#include "utl/Loader.h"

class DataLoader;
typedef void (DataLoader::*DataLoaderStateFunc)(void);

class DataLoader : public Loader {
public:
    DataLoader(const FilePath &, LoaderPos, bool);
    virtual ~DataLoader();
    virtual bool IsLoaded() const;
    virtual const char *StateName() const { return "DataLoader"; }
    virtual void PollLoading();
    virtual void OpenFile();

    DataArray *Data();
    void LoadFile();
    void DoneLoading();
    void ThreadDone(DataArray *);

    // String unk18;
    // DataArray *unk24;
    // File *fileobj;
    // int filesize; // 0x2C
    // void *unk30;
    // bool unk34;
    // class DataLoaderThreadObj *unk38;
    // DataLoaderStateFunc ptmf; // 0x3C
};

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
