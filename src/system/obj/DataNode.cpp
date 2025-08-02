#include "Data.h"
#include "obj/Data.h"
#include "os/Debug.h"
#include "utl/TextStream.h"
#include "obj/DataFunc.h"
#include "obj/Object.h"
#include <cstring>
#include <map>

int gEvalIndex;
std::map<Symbol, DataNode> gDataVars;
DataNode gEvalNode[8];

bool DataNode::CompatibleType(DataType ty) const {
    if (mType == ty)
        return true;
    switch (mType) {
    case kDataInt:
        return ty == kDataFloat;
    case kDataSymbol:
        return ty == kDataString || ty == kDataObject;
    case kDataString:
        return ty == kDataObject;
    default:
        return false;
    }
    return true;
}

const char *DataNode::DataTypeString(DataType ty) {
    switch (ty) {
    case 0:
        return "kDataInt";
    case 1:
        return "kDataFloat";
    case 2:
        return "kDataVar";
    case 3:
        return "kDataFunc";
    case 4:
        return "kDataObject";
    case 5:
        return "kDataSymbol";
    case 6:
        return "kDataUnhandled";
    case 0x10:
        return "kDataArray";
    case 0x11:
        return "kDataCommand";
    case 0x12:
        return "kDataString";
    case 0x13:
        return "kDataProperty";
    case 0x14:
        return "kDataGlob";
    case 7:
        return "kDataIfdef";
    case 8:
        return "kDataElse";
    case 9:
        return "kDataEndif";
    case 0x20:
        return "kDataDefine";
    case 0x21:
        return "kDataInclude";
    case 0x22:
        return "kDataMerge";
    case 0x23:
        return "kDataIfndef";
    case 0x24:
        return "kDataAutorun";
    case 0x25:
        return "kDataUndef";
    default:
        return "Unknown data type";
    }
}

DataNode::DataNode(const char *c) {
    mValue.array = new DataArray(c, strlen(c) + 1);
    mType = kDataString;
}

DataNode::DataNode(const String &str) {
    mValue.array = new DataArray(str.c_str(), str.length() + 1);
    mType = kDataString;
}

DataNode::DataNode(const DataNode &node) {
    mValue = node.mValue;
    mType = node.mType;
    if (mType & kDataArray)
        mValue.array->AddRef();
}

DataNode::DataNode(const DataArrayPtr &ptr) {
    mValue.array = ptr;
    ptr->AddRef();
    mType = kDataArray;
}

DataNode::DataNode(DataArray *array, DataType type) {
    MILO_ASSERT(array, 0x158);
    mValue.array = array;
    mValue.array->AddRef();
    MILO_ASSERT(type & kDataArray, 0x15B);
    mType = type;
}

DataNode &DataNode::operator=(const DataNode &node) {
    if (&node != this) {
        if (mType & kDataArray)
            mValue.array->Release();
        mValue = node.mValue;
        mType = node.mType;
        if (mType & kDataArray)
            mValue.array->AddRef();
    }
    return *this;
}

const DataNode &UseQueue(const DataNode &node) {
    int i;
    gEvalNode[gEvalIndex] = node;
    i = gEvalIndex;
    gEvalIndex = gEvalIndex + 1 & 7;
    return gEvalNode[i];
}

// const DataNode &DataNode::Evaluate() const {
//     if (mType == kDataCommand) {
//         DataNode lol = mValue.array->Execute();
//         return UseQueue(lol);
//     } else if (mType == kDataVar) {
//         return *mValue.var;
//     } else if (mType == kDataProperty) {
//         MILO_ASSERT(gDataThis, 0x78);
//         const DataNode *n = gDataThis->Property(mValue.array, true);
//         return UseQueue(*n);
//     } else
//         return *this;
// }

bool DataNode::NotNull() const {
    const DataNode &n = Evaluate();
    DataType t = n.Type();
    if (t == kDataSymbol) {
        return n.mValue.symbol[0] != 0;
    } else if (t == kDataString) {
        return n.mValue.array->Size() < -1;
    } else if (t == kDataGlob) {
        return n.mValue.array->Size() & -1;
    } else
        return n.mValue.array != 0;
}

bool DataVarExists(Symbol s) { return gDataVars.find(s) != gDataVars.end(); }

const char *DataVarName(const DataNode *node) {
    for (std::map<Symbol, DataNode>::iterator it = gDataVars.begin();
         it != gDataVars.end();
         it++) {
        if ((&it->second) == node) {
            return it->first.Str();
        }
    }
    return "<null>";
}

inline bool HasChar(const char *str, char c) {
    while (*str != '\0') {
        if (*str++ == c)
            return true;
    }
    return false;
}

void DataNode::Print(TextStream &ts, bool b, int i3) const {
    switch (mType) {
    case kDataUnhandled:
        ts << "kDataUnhandled";
        break;
    case kDataInt:
        ts << mValue.integer;
        break;
    case kDataString:
        if (b)
            ts << mValue.var->mValue.symbol;
        else {
            ts << "\"";
            char *tok = strtok((char *)mValue.var->mValue.symbol, "\"");
            while (tok) {
                ts << tok;
                tok = strtok(0, "\"");
                if (tok) {
                    ts << "\\q";
                    tok[-1] = '\"';
                }
            }
            ts << "\"";
        }
        break;
    case kDataSymbol:
        if (!HasChar(mValue.symbol, ' ') && !HasChar(mValue.symbol, '#')
            && !HasChar(mValue.symbol, '-'))
            ts << mValue.symbol;
        else
            ts << "'" << mValue.symbol << "'";
        break;
    case kDataGlob:
        ts << "<glob " << -mValue.array->Size() << ">";
        break;
    case kDataFloat:
        ts << mValue.real;
        break;
    case kDataArray:
    case kDataCommand:
    case kDataProperty:
        mValue.array->Print(ts, mType, b, i3);
        break;
    case kDataObject:
        ts << mValue.object;
        break;
    case kDataVar:
        ts << '$' << DataVarName(mValue.var);
        break;
    case kDataFunc:
        ts << DataFuncName(mValue.func);
        break;
    case kDataDefine:
        ts << "\n#define " << mValue.symbol << "\n";
        break;
    case kDataUndef:
        ts << "\n#undef " << mValue.symbol << "\n";
        break;
    case kDataIfdef:
        ts << "\n#ifdef " << mValue.symbol << "\n";
        break;
    case kDataIfndef:
        ts << "\n#ifndef " << mValue.symbol << "\n";
        break;
    case kDataAutorun:
        ts << "\n#autorun\n";
        break;
    case kDataElse:
        ts << "\n#else\n";
        break;
    case kDataEndif:
        ts << "\n#endif\n";
        break;
    case kDataInclude:
        ts << "\n#include " << mValue.symbol << "\n";
        break;
    case kDataMerge:
        ts << "\n#merge " << mValue.symbol << "\n";
        break;
    }
}

bool DataNode::PrintUnused(TextStream &, bool) const {
    MILO_WARN("Enable PRINT_UNUSED in Data.h to utilize DataNode::PrintUnused()");
    return false;
}

void DataNode::Save(BinStream &d) const {
    d << mType;
    switch (mType) {
    case kDataSymbol:
    case kDataIfdef:
    case kDataDefine:
    case kDataInclude:
    case kDataMerge:
    case kDataIfndef:
    case kDataUndef:
        d << mValue.symbol;
        break;
    case kDataFloat:
        d << mValue.real;
        break;
    case kDataString:
    case kDataGlob:
        mValue.array->SaveGlob(d, (mType - kDataString) == 0);
        break;
    case kDataArray:
    case kDataCommand:
    case kDataProperty:
        mValue.array->Save(d);
        break;
    case kDataObject:
        if (mValue.object)
            d << mValue.object->Name();
        else
            d << "";
        break;
    case kDataVar:
        d << DataVarName(mValue.var);
        break;
    case kDataFunc:
        d << DataFuncName(mValue.func);
        break;
    case kDataUnhandled:
    case kDataInt:
    case kDataElse:
    case kDataEndif:
    case kDataAutorun:
        d << mValue.integer;
        break;
    default:
        MILO_FAIL("Unrecognized node type: %x", mType);
        break;
    }
}

int DataNode::Int(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType != kDataInt) {
        String s;
        n.Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Int (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Int", s);
    }
    return n.mValue.integer;
}

int DataNode::LiteralInt(const DataArray *source) const {
    if (mType != kDataInt) {
        String s;
        Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Int (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Int", s);
    }
    return mValue.integer;
}

Symbol DataNode::Sym(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType != kDataSymbol) {
        String s;
        n.Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Symbol (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Symbol", s);
    }
    return STR_TO_SYM(n.mValue.symbol);
}

Symbol DataNode::LiteralSym(const DataArray *source) const {
    if (mType != kDataSymbol) {
        String s;
        Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Symbol (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Symbol", s);
    }
    return STR_TO_SYM(mValue.symbol);
}

Symbol DataNode::ForceSym(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType == kDataSymbol) {
        return STR_TO_SYM(n.mValue.symbol);
    } else {
        if (n.mType != kDataString) {
            String s;
            n.Print(s, true, 0);
            if (source)
                MILO_FAIL(
                    "Data %s is not String (file %s, line %d)",
                    s.c_str(),
                    source->File(),
                    source->Line()
                );
            else
                MILO_FAIL("Data %s is not String", s);
        }
        return Symbol(n.mValue.var->mValue.symbol);
    }
}

const char *DataNode::Str(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType == kDataSymbol) {
        return n.mValue.symbol;
    } else {
        if (n.mType != kDataString) {
            String s;
            n.Print(s, true, 0);
            if (source)
                MILO_FAIL(
                    "Data %s is not String (file %s, line %d)",
                    s.c_str(),
                    source->File(),
                    source->Line()
                );
            else
                MILO_FAIL("Data %s is not String", s);
        }
        return n.mValue.var->mValue.symbol;
    }
}

const char *DataNode::LiteralStr(const DataArray *source) const {
    if (mType == kDataSymbol) {
        return mValue.symbol;
    } else {
        if (mType != kDataString) {
            String s;
            Print(s, true, 0);
            if (source)
                MILO_FAIL(
                    "Data %s is not String (file %s, line %d)",
                    s.c_str(),
                    source->File(),
                    source->Line()
                );
            else
                MILO_FAIL("Data %s is not String", s);
        }
        return mValue.var->mValue.symbol;
    }
}

float DataNode::Float(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType == kDataInt) {
        return n.mValue.integer;
    } else {
        if (n.mType != kDataFloat) {
            String s;
            n.Print(s, true, 0);
            if (source)
                MILO_FAIL(
                    "Data %s is not Float (file %s, line %d)",
                    s.c_str(),
                    source->File(),
                    source->Line()
                );
            else
                MILO_FAIL("Data %s is not Float", s);
        }
        return n.mValue.real;
    }
}

float DataNode::LiteralFloat(const DataArray *source) const {
    if (mType == kDataInt) {
        return mValue.integer;
    } else {
        if (mType != kDataFloat) {
            String s;
            Print(s, true, 0);
            if (source)
                MILO_FAIL(
                    "Data %s is not Float (file %s, line %d)",
                    s.c_str(),
                    source->File(),
                    source->Line()
                );
            else
                MILO_FAIL("Data %s is not Float", s);
        }
        return mValue.real;
    }
}

DataFunc *DataNode::Func(const DataArray *source) const {
    if (mType != kDataFunc) {
        String s;
        Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Func (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Func", s);
    }
    return mValue.func;
}

// public: class Hmx::Object * __cdecl DataNode::GetObj(class DataArray const *) const

DataArray *DataNode::Array(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType != kDataArray) {
        String s;
        n.Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Array (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Array", s);
    }
    return n.mValue.array;
}

DataArray *DataNode::LiteralArray(const DataArray *source) const {
    if (mType != kDataArray) {
        String s;
        Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Array (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Array", s);
    }
    return mValue.array;
}

DataArray *DataNode::Command(const DataArray *source) const {
    if (mType != kDataCommand) {
        String s;
        Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Command (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Command", s);
    }
    return mValue.array;
}

DataNode *DataNode::Var(const DataArray *source) const {
    if (mType != kDataVar) {
        String s;
        Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Var (file %s, line %d)",
                s.c_str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Var", s);
    }
    return mValue.var;
}

bool DataNode::operator>(const DataNode &other) const {
    if ((mType == kDataInt || mType == kDataFloat)
        && (other.mType == kDataInt || other.mType == kDataFloat)) {
        return LiteralFloat() > other.LiteralFloat();
    } else
        return false;
}

bool DataNode::operator!=(const DataNode &other) const {
    return !Equal(other, nullptr, true);
}

DataNode &DataVariable(Symbol s) { return gDataVars[s]; }

void DataNode::Load(BinStream &d) {
    static char buf[128];
    d >> (int &)mType;
    switch (mType) {
    case kDataFunc: {
        Symbol sym;
        d >> sym;
        const std::map<Symbol, DataFunc *>::iterator it = gDataFuncs.find(sym);
        if (it == gDataFuncs.end()) {
            MILO_FAIL("Couldn't bind %s", sym);
        }
        mValue.func = it->second;
        break;
    }
    case kDataSymbol:
    case kDataIfdef:
    case kDataDefine:
    case kDataInclude:
    case kDataMerge:
    case kDataIfndef:
    case kDataUndef: {
        Symbol sym;
        d >> sym;
        mValue.symbol = sym.Str();
        break;
    }
    case kDataFloat:
        d >> mValue.real;
        break;
    case kDataString:
    case kDataGlob:
        mValue.array = new DataArray(0);
        mValue.array->LoadGlob(d, (mType - kDataString) == 0);
        break;
    case kDataArray:
    case kDataCommand:
    case kDataProperty:
        mValue.array = new DataArray(0);
        mValue.array->Load(d);
        break;
    case kDataObject:
        // d.ReadString(buf, 0x80);
        // mValue.object = gDataDir->FindObject(buf, true);
        //         if (mValue.object == 0 && *buf) {
        //             MILO_WARN("Couldn't find %s from %s", buf, gDataDir->Name());
        //         }
        break;
    case kDataVar: {
        Symbol sym;
        d >> sym;
        mValue.var = &gDataVars[sym];
        break;
    }
    case kDataUnhandled:
        mType = kDataInt;
        d >> mValue.integer;
        break;
    case kDataInt:
        mType = kDataUnhandled;
        d >> mValue.integer;
        break;
    case kDataElse:
    case kDataEndif:
    case kDataAutorun:
        d >> mValue.integer;
        break;
    default:
        MILO_FAIL("Unrecognized node type: %x", mType);
        break;
    }
}
