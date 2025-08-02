#include "obj/Data.h"
#include "os/Debug.h"
#include "utl/BinStream.h"
#include "utl/MemMgr.h"
#include "utl/Str.h"

bool DataArray::PrintUnused(TextStream &ts, DataType ty, bool b) const {
    bool ret = false;
    for (int i = 0; i < mSize; i++) {
        ret |= mNodes[i].PrintUnused(ts, b);
    }
    return ret;
}

DataArray::~DataArray() {
    int i2;
    if (mSize < 0) {
        i2 = -mSize;
    } else {
        for (int i = 0; i < mSize; i++) {
            mNodes[i].~DataNode();
        }
        i2 = mSize * 8;
    }
    MemOrPoolFree(i2, mNodes, "unknown", 0, "unknown");
}

void DataArray::SetFileLine(Symbol file, int line) {
    mFile = file;
    mLine = line;
}

void DataArray::SetFile(Symbol file) { gFile = file; }

bool strncat_tofit(FixedString &str, const char *cc, int i) {
    if ((int)(strlen(str.c_str()) + strlen(cc)) < i) {
        str += cc;
        return true;
    } else
        return false;
}

DataNode &DataArray::Node(int i) const {
    MILO_ASSERT_FMT(
        i >= 0 && i < mSize,
        "Array doesn't have node %d, only has %d (file %s, line %d)",
        i,
        mSize,
        File(),
        Line()
    );
    return mNodes[i];
}

DataNode &DataArray::Node(int i) {
    MILO_ASSERT_FMT(
        i >= 0 && i < mSize,
        "Array doesn't have node %d, only has %d (file %s, line %d)",
        i,
        mSize,
        File(),
        Line()
    );
    return mNodes[i];
}

void DataArray::Print(TextStream &ts, DataType type, bool b, int i3) const {
    DataNode *end = &mNodes[mSize];
    MILO_ASSERT(type & kDataArray, 0xA3);
    char open = '\0';
    char close = '\0';
    if (type == kDataArray) {
        open = '(';
        close = ')';
    } else if (type == kDataCommand) {
        open = '{';
        close = '}';
    } else if (type == kDataProperty) {
        open = '[';
        close = ']';
    } else {
        MILO_FAIL("Unrecognized array type %d", type);
    }

    DataNode *n;
    for (n = mNodes; n < end && !(n->Type() & kDataArray); n++)
        ;

    if (n == end || b) {
        ts << open;
        for (n = mNodes; n < end; n++) {
            if (n != mNodes) {
                ts << " ";
            }
            n->Print(ts, b, i3);
        }
    } else {
        ts << open;
        n = mNodes;
        if (n->Type() == kDataSymbol) {
            n->Print(ts, b, i3);
            n++;
        }
        ts << "\n";
        i3 += 3;
        for (; n < end; n++) {
            ts.Space(i3);
            n->Print(ts, b, i3);
            ts << "\n";
        }
        i3 -= 3;
        ts.Space(i3);
    }
    ts << close;
}

DataArray::DataArray(int size)
    : mFile(), mSize(size), mRefs(1), mLine(0), mDeprecated(0) {
    mNodes = (DataNode *)MemOrPoolAlloc(size * 8, __FILE__, 0xFE, "Nodes");
    for (int n = 0; n < size; n++) {
        new (&mNodes[n]) DataNode();
    }
}

DataArray::DataArray(const void *glob, int size)
    : mFile(), mSize(-size), mRefs(1), mLine(0), mDeprecated(0) {
    mNodes = (DataNode *)MemOrPoolAlloc(size, __FILE__, 0xFE, "Nodes");
    memcpy(mNodes, glob, size);
}

void DataArray::Save(BinStream &bs) const {
    bs << mSize << mLine << mDeprecated;
    for (int i = 0; i < mSize; i++) {
        mNodes[i].Save(bs);
    }
}

void DataArray::SaveGlob(BinStream &bs, bool b) const {
    if (b) {
        int i = -1 - mSize;
        bs << i;
        bs.Write(mNodes, i);
    } else {
        bs << mSize;
        bs.Write(mNodes, -mSize);
    }
}

void DataArray::LoadGlob(BinStream &bs, bool b) {
    MILO_ASSERT(mSize <= 0, 0x4A8);
    MemOrPoolFree(-mSize, mNodes);
    if (b) {
        int v;
        bs >> v;
        mSize = -(v + 1);
        mNodes = (DataNode *)MemOrPoolAlloc(-mSize, __FILE__, 0xFE, "Nodes");
        bs.Read(mNodes, v);
        ((unsigned char *)mNodes)[v] = 0;
    } else {
        bs >> mSize;
        mNodes = (DataNode *)MemOrPoolAlloc(-mSize, __FILE__, 0xFE, "Nodes");
        bs.Read(mNodes, -mSize);
    }
}

TextStream &operator<<(TextStream &ts, const DataArray *da) {
    if (da)
        da->Print(ts, kDataArray, false, 0);
    else
        ts << "<null>";
    return ts;
}

BinStream &operator<<(BinStream &bs, const DataArray *da) {
    if (da != 0) {
        bs << true;
        da->Save(bs);
    } else
        bs << false;
    return bs;
}

DataArray *DataArray::FindArray(int tag, bool fail) const {
    DataNode *dn;
    DataNode *dn_end = &mNodes[mSize];
    for (dn = mNodes; dn < dn_end; dn++) {
        if (dn->Type() == kDataArray) {
            const DataArray *arr = dn->UncheckedArray();
            if (arr->UncheckedInt(0) == tag) {
                return (DataArray *)arr;
            }
        }
    }
    if (fail)
        MILO_FAIL("Couldn't find %d in array (file %s, line %d)", tag, File(), mLine);
    return nullptr;
}

DataArray *DataArray::FindArray(Symbol tag, bool fail) const {
    DataArray *found = FindArray((int)tag, false);
    if (!found && fail)
        MILO_FAIL(
            "Couldn't find '%s' in array (file %s, line %d)", tag.Str(), File(), mLine
        );
    return found;
}

DataArray *DataArray::FindArray(Symbol s1, Symbol s2) const {
    return FindArray(s1)->FindArray(s2);
}

DataArray *DataArray::FindArray(Symbol s1, Symbol s2, Symbol s3) const {
    return FindArray(s1)->FindArray(s2)->FindArray(s3);
}

DataArray *DataArray::FindArray(Symbol s, const char *c) const {
    return FindArray(s, Symbol(c));
}

bool DataArray::FindData(Symbol s, const char *&ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != 0) {
        ret = arr->Str(1);
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, Symbol &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = (arr->Sym(1));
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, String &ret, bool b) const {
    const char *c;
    bool found = FindData(s, c, b);
    if (found) {
        ret = c;
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, int &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = arr->Int(1);
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, float &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = arr->Float(1);
        return true;
    } else {
        return false;
    }
}

bool DataArray::FindData(Symbol s, bool &ret, bool b) const {
    DataArray *arr = FindArray(s, b);
    if (arr != nullptr) {
        ret = arr->Int(1);
        return true;
    } else {
        return false;
    }
}

BinStream &operator>>(BinStream &bs, DataArray *&da) {
    bool b;
    bs >> b;
    if (b) {
        da = new DataArray(0);
        da->Load(bs);
    } else
        da = nullptr;
    return bs;
}
