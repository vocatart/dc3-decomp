#include "os/ArkFile_p.h"
#include "os/Archive.h"
#include "os/File.h"

int ArkFile::Read(void *c, int a) {
    if (ReadAsync(c, a) == 0)
        return 0;
    int ret = -1;
    while (ReadDone(ret) == 0)
        ;
    return ret;
}

int ArkFile::Seek(int offset, int mode) {
    switch (mode) {
    case 0:
        mTell = offset;
        break;
    case 1:
        mTell += offset;
        break;
    case 2:
        mTell = mSize + offset;
        break;
    default:
        break;
    }
    return mTell;
}

bool ArkFile::Eof() { return (mSize - mTell) == 0; }
bool ArkFile::Fail() { return mFail; }

void ArkFile::TaskDone(int a) {
    mNumOutstandingTasks--;
    mBytesRead += a;
    mTell += a;
}

bool ArkFile::ReadDone(int &i) {
    // TheBlockMgr.Poll();
    i = mBytesRead;
    return mNumOutstandingTasks == 0;
}

ArkFile::ArkFile(const char *iFilename, int iMode)
    : mArkfileNum(0), mByteStart(0), mReadStartTime(0), mSize(0), mUCSize(0),
      mNumOutstandingTasks(0), mBytesRead(0), mTell(0), mFail(false), mReadAhead(true),
      mFilename(iFilename) {
    if (!TheArchive->GetFileInfo(
            FileMakePath(".", iFilename), mArkfileNum, mByteStart, mSize, mUCSize
        )
        || (iMode & 1)) {
        mFail = true;
    }
}

ArkFile::~ArkFile() {}

int ArkFile::Write(const void *, int) {
    MILO_FAIL("ERROR: Cannot write to a file in an archive!");
    return 0;
}
