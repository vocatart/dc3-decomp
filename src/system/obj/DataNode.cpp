#include "Data.h"
#include "obj/Data.h"
#include "os/Debug.h"
#include "utl/TextStream.h"
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

const char* DataNode::DataTypeString(DataType ty){
    switch(ty) {
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

bool DataVarExists(Symbol s) {
    return gDataVars.find(s) != gDataVars.end();
}

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

bool DataNode::PrintUnused(TextStream&, bool) const {
    MILO_WARN("Enable PRINT_UNUSED in Data.h to utilize DataNode::PrintUnused()");
    return false;
}

int DataNode::Int(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType != kDataInt) {
        String s;
        n.Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Int (file %s, line %d)",
                s.Str(),
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
                s.Str(),
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
                s.Str(),
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
                s.Str(),
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
                    s.Str(),
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
                    s.Str(),
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
                    s.Str(),
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
                    s.Str(),
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
                    s.Str(),
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
                s.Str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Func", s);
    }
    return mValue.func;
}

DataArray *DataNode::Array(const DataArray *source) const {
    const DataNode &n = Evaluate();
    if (n.mType != kDataArray) {
        String s;
        n.Print(s, true, 0);
        if (source)
            MILO_FAIL(
                "Data %s is not Array (file %s, line %d)",
                s.Str(),
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
                s.Str(),
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
                s.Str(),
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
                s.Str(),
                source->File(),
                source->Line()
            );
        else
            MILO_FAIL("Data %s is not Var", s);
    }
    return mValue.var;
}

bool DataNode::operator>(const DataNode& other) const {
    if((mType == kDataInt || mType == kDataFloat) && (other.mType == kDataInt || other.mType == kDataFloat)){
        return LiteralFloat() > other.LiteralFloat();
    }
    else return false;
}

bool DataNode::operator!=(const DataNode& other) const {
    return !Equal(other, nullptr, true);
}

DataNode &DataVariable(Symbol s) { return gDataVars[s]; }
