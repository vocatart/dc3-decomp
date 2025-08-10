#include "obj/DataFile.h"
#include "obj/DataFile_Flex.h"
#include "os/CritSec.h"
#include "os/Debug.h"

CriticalSection gDataReadCrit; // yes these are the bss offsets. this tu sucks
// DataArray *gArray; // 0x28
// int gNode; // 0x2c
Symbol gFile; // 0x30
// BinStream *gBinStream; // 0x34
// int gOpenArray = kDataTokenFinished; // 0x38 ?
// std::list<ConditionalInfo> gConditional; // 0x48 - actually a list of ConditionalInfo
//                                          // structs
int gDataLine; // 0x50
// std::map<String, DataNode> gReadFiles; // 0x60

// bool gCompressCached;
// bool gCachingFile;
// bool gReadingFile;

DataArray *ReadCacheStream(BinStream &bs, const char *cc) {
    CritSecTracker cst(&gDataReadCrit);
    bs.EnableReadEncryption();
    DataArray::SetFile(cc);
    DataArray *arr;
    bs >> arr;
    bs.DisableEncryption();
    return arr;
}

void DataFail(const char *x) { MILO_FAIL("%s (file %s, line %d)", x, gFile, gDataLine); }
