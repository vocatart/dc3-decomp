#pragma once
#include "obj/Data.h"
#include "utl/Str.h"
#include <map>

class DataPoint {
private:
    Symbol mType; // 0x0
    std::map<Symbol, DataNode> mNameValPairs; // 0x4

public:
    DataPoint();
    DataPoint(const char *);

    void AddPair(const char *, DataNode);
    void AddPair(Symbol, DataNode);
    void ToJSON(String &) const;
};

typedef void DataPointRecordFunc(DataPoint &);

class DataPointMgr {
private:
    DataPointRecordFunc *mDataPointRecorder; // 0x0
    DataPointRecordFunc *mDebugDataPointRecorder; // 0x4
    const char *mHostName; // 0x8
    const char *mApp; // 0xc
    const char *mProject; // 0x10
    int mVersion; // 0x14
    String unk18; // 0x18
    String unk20; // 0x20
    String unk28; // 0x28

public:
    DataPointMgr();
    ~DataPointMgr();

    DataPointRecordFunc *SetDataPointRecorder(DataPointRecordFunc *);
    void RecordDataPoint(DataPoint &);
    void RecordDebugDataPoint(DataPoint &);
    void Init();
};

extern DataPointMgr &TheDataPointMgr;
