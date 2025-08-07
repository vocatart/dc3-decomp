#include "obj/DataFunc.h"
#include "math/Decibels.h"
#include "math/Utl.h"
#include "obj/Data.h"
#include "obj/Object.h"
#include "os/Debug.h"

bool SwitchMatch(const DataNode &n1, const DataNode &n2) {
    if (n1.Type() == kDataArray) {
        DataArray *arr = n1.UncheckedArray();
        for (int i = 0; i < arr->Size(); i++) {
            DataNode &cur = arr->Node(i);
            if (cur.Equal(n2, nullptr, true)) {
                return true;
            }
        }
        return false;
    } else
        return n1.Equal(n2, nullptr, true);
}

DataNode DataFuncObj::New(DataArray *arr) {
    Hmx::Object *o = ObjectDir::Main()->Find<Hmx::Object>(arr->Str(1), false);
    if (o)
        delete o;
    return new DataFuncObj(arr);
}

DEF_DATA_FUNC(DataSprintf) {
    FormatString fs(array->Str(1));
    for (int i = 2; i < array->Size(); i++) {
        fs << array->Evaluate(i);
    }
    return fs.Str();
}

DEF_DATA_FUNC(DataPrintf) {
    FormatString fs(array->Str(1));
    for (int i = 2; i < array->Size(); i++) {
        fs << array->Evaluate(i);
    }
    TheDebug << fs.Str();
    return 0;
}

DEF_DATA_FUNC(DataPrint) {
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(TheDebug, true, 0);
    }
    return 0;
}

DEF_DATA_FUNC(DataSprint) {
    class String str;
    for (int i = 1; i < array->Size(); i++) {
        array->Evaluate(i).Print(str, true, 0);
    }
    return str.c_str();
}

DEF_DATA_FUNC(DataSet) {
    const DataArray *a = array;
    DataNode ret(a->Evaluate(2));
    if (a->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0xA9);
        gDataThis->SetProperty(a->Node(1).UncheckedArray(), ret);
    } else
        *a->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataSetVar) {
    DataNode ret = array->Evaluate(2);
    DataVariable(array->ForceSym(1)) = ret;
    return ret;
}

DEF_DATA_FUNC(DataIfElse) {
    MILO_ASSERT_FMT(
        array->Size() == 4,
        "One condition and two alternatives expected (file %s, line %d)",
        array->File(),
        array->Line()
    );
    if (array->Node(1).NotNull())
        return array->Evaluate(2);
    else
        return array->Evaluate(3);
}

DEF_DATA_FUNC(DataIf) {
    if (array->Node(1).NotNull()) {
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
        return 0;
    } else
        return 0;
}

DEF_DATA_FUNC(DataUnless) {
    if (!array->Node(1)) {
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
        return 0;
    }
    return 0;
}

/** Verifies if two DataNodes are equivalent. */
DEF_DATA_FUNC(DataEq) {
    const DataNode &dn1 = array->Evaluate(1);
    const DataNode &dn2 = array->Evaluate(2);
    return dn1.Equal(dn2, array, true);
}

DEF_DATA_FUNC(DataFindElem) {
    DataArray *arr = array->Array(1);
    arr->AddRef();
    const DataNode &dn = array->Evaluate(2);
    for (int i = 0; i < arr->Size(); i++) {
        DataNode &arrNode = arr->Node(i);
        if (arrNode.Equal(dn, nullptr, true)) {
            if (array->Size() > 3) {
                *array->Var(3) = i;
            }
            arr->Release();
            return 1;
        }
    }
    arr->Release();
    return 0;
}

/** Verifies if two DataNodes are NOT equivalent. */
DEF_DATA_FUNC(DataNe) { return DataEq(array).UncheckedInt() == 0; }

DEF_DATA_FUNC(DataLe) { return array->Float(1) <= array->Float(2); }

DEF_DATA_FUNC(DataLt) { return array->Float(1) < array->Float(2); }

DEF_DATA_FUNC(DataGe) { return array->Float(1) >= array->Float(2); }

DEF_DATA_FUNC(DataGt) { return array->Float(1) > array->Float(2); }

DEF_DATA_FUNC(DataDbToRatio) {
    float f1 = array->Float(1);
    float f2 = -96 - f1 >= 0 ? -96 : f1;
    f1 = f2 >= 0 ? 0 : f2;
    return DbToRatio(f1);
}

DEF_DATA_FUNC(DataRatioToDb) {
    float f1 = array->Float(1);
    float f2;
    f2 = -f1 >= 0 ? 0 : f1;
    f1 = f2 - 1 >= 0 ? 1 : f2;
    return RatioToDb(f1);
}

DEF_DATA_FUNC(DataNot) { return !array->Node(1); }

DEF_DATA_FUNC(DataAnd) {
    for (int i = 1; i < array->Size(); i++) {
        if (!array->Node(i)) {
            return 0;
        }
    }
    return 1;
}

DEF_DATA_FUNC(DataOr) {
    for (int i = 1; i < array->Size(); i++) {
        if (array->Node(i).NotNull())
            return 1;
    }
    return 0;
}

DEF_DATA_FUNC(DataXor) { return array->Node(1).NotNull() != array->Node(2).NotNull(); }

DEF_DATA_FUNC(DataBitAnd) {
    MILO_ASSERT(array->Size() >= 3, 0x146);
    int res = array->Int(1);
    for (int i = 2; i < array->Size(); i++) {
        res &= array->Int(i);
    }
    return res;
}

DEF_DATA_FUNC(DataAndEqual) {
    MILO_ASSERT(array->Size() == 3, 0x152);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x157);
        DataArray *arr = array->UncheckedArray(1);
        int res = gDataThis->Property(arr, true)->Int() & array->Int(2);
        gDataThis->SetProperty(arr, res);
        return res;
    } else {
        DataNode *var = array->Var(1);
        int res = var->Int() & array->Int(2);
        return (*var = res);
    }
}

DEF_DATA_FUNC(DataMaskEqual) {
    MILO_ASSERT(array->Size() == 3, 0x168);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x16D);
        DataArray *arr = array->UncheckedArray(1);
        int res = gDataThis->Property(arr, true)->Int() & ~array->Int(2);
        gDataThis->SetProperty(arr, res);
        return res;
    } else {
        DataNode *var = array->Var(1);
        int res = var->Int() & ~array->Int(2);
        return (*var = res);
    }
}

DEF_DATA_FUNC(DataOrEqual) {
    MILO_ASSERT(array->Size() == 3, 0x17F);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x184);
        DataArray *arr = array->UncheckedArray(1);
        int res = gDataThis->Property(arr, true)->Int() | array->Int(2);
        gDataThis->SetProperty(arr, res);
        return res;
    } else {
        DataNode *var = array->Var(1);
        int res = var->Int() | array->Int(2);
        return (*var = res);
    }
}

DEF_DATA_FUNC(DataBitOr) {
    MILO_ASSERT(array->Size() >= 3, 0x196);
    int res = array->Int(1);
    for (int i = 2; i < array->Size(); i++)
        res |= array->Int(i);
    return res;
}

DEF_DATA_FUNC(DataBitXor) { return array->Int(1) ^ array->Int(2); }

DEF_DATA_FUNC(DataBitNot) { return ~array->Int(1); }

DEF_DATA_FUNC(DataLowestBit) { return LowestBit(array->Int(1)); }

DEF_DATA_FUNC(DataCountBits) { return CountBits(array->Int(1)); }

DEF_DATA_FUNC(DataWhile) {
    while (array->Node(1).NotNull()) {
        for (int i = 2; i < array->Size(); i++) {
            array->Command(i)->Execute(true);
        }
    }
    return 0;
}

DEF_DATA_FUNC(DataVar) { return &DataVariable(array->ForceSym(1)); }

DEF_DATA_FUNC(DataPackColor) {
    float b = array->Float(3);
    float g = array->Float(2);
    float r = array->Float(1);

    int r_packed = ((int)(r * 255.0f) & 0xFF);
    int g_packed = ((int)(g * 255.0f) & 0xFF) << 8;
    int b_packed = ((int)(b * 255.0f) & 0xFF) << 16;

    return r_packed | g_packed | b_packed;
}

DEF_DATA_FUNC(DataUnpackColor) {
    int packed = array->Int(1);
    *array->Var(2) = (float)(packed & 0xFF) / 255.0f;
    *array->Var(3) = (float)(packed >> 8 & 0xFF) / 255.0f;
    *array->Var(4) = (float)(packed >> 0x10 & 0xFF) / 255.0f;
    return 0;
}

DEF_DATA_FUNC(DataDo) {
    int size = array->Size(); // this needs to be up here to match
    int i;
    for (i = 1; array->Type(i) == kDataArray; i++) {
        DataArray *binding = array->UncheckedArray(i);
        DataNode *n = binding->Var(0);
        DataPushVar(n);
        if (binding->Size() == 2) {
            *n = binding->Evaluate(1);
        } else {
            MILO_ASSERT_FMT(binding->Size() == 1, "do var has more than one initializer");
        }
    }
    int numVars = i - 1;

    for (; i < size - 1; i++) {
        array->Command(i)->Execute(true);
    }
    DataNode ret(array->Evaluate(i));

    while (numVars-- != 0) {
        DataPopVar();
    }

    return ret;
}

DEF_DATA_FUNC(DataMin) {
    int min = -1;
    float minf; // this matches but wow this is not good
    for (int i = 1; i < array->Size(); i++) {
        float f = array->Float(i);
        if (min < 0 || f < minf) {
            minf = f;
            min = i;
        }
    }
    if (min > 0) {
        return array->Evaluate(min);
    }
    MILO_FAIL("DataMin: No valid data to compare");
    return 0;
}

DEF_DATA_FUNC(DataMax) {
    int max = -1;
    float maxf;
    for (int i = 1; i < array->Size(); i++) {
        float f = array->Float(i);
        if (max < 0 || f > maxf) {
            maxf = f;
            max = i;
        }
    }
    if (max > 0) {
        return array->Evaluate(max);
    }
    MILO_FAIL("DataMax: No valid data to compare");
    return 0;
}

DEF_DATA_FUNC(DataAbs) {
    const DataNode &n = array->Evaluate(1);
    float f = std::fabs(n.LiteralFloat(array));
    if (n.Type() == kDataInt)
        return (int)f;
    else
        return f;
}

DEF_DATA_FUNC(DataAdd) {
    float sum_f = 0.0f;
    int sum_int = 0;
    int cnt = array->Size();
    int i;
    for (i = 1; i < cnt; i++) {
        const DataNode &n = array->Evaluate(i);
        if (n.Type() != kDataInt) {
            sum_f = sum_int + n.LiteralFloat(array);
            break;
        }
        sum_int += n.UncheckedInt();
    }
    if (i == cnt)
        return sum_int;
    for (i++; i < cnt; i++) {
        sum_f += array->Float(i);
    }
    return sum_f;
}

DEF_DATA_FUNC(DataAddEq) {
    DataNode ret = DataAdd(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x24F);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataSub) {
    const DataNode &dn = array->Evaluate(1);
    if (array->Size() == 2) {
        if (dn.Type() == kDataFloat) {
            return -dn.LiteralFloat(array);
        } else
            return -dn.LiteralInt(array);
    } else {
        const DataNode &dn2 = array->Evaluate(2);
        if (dn.Type() == kDataFloat || dn2.Type() == kDataFloat) {
            return dn.LiteralFloat(array) - dn2.LiteralFloat(array);
        } else {
            return dn.LiteralInt(array) - dn2.LiteralInt(array);
        }
    }
}

DEF_DATA_FUNC(DataMean) {
    float sum = 0.0;
    int cnt = array->Size();
    for (int i = 1; i < cnt; i++) {
        sum += array->Float(i);
    }
    return sum / (cnt - 1);
}

DEF_DATA_FUNC(DataClamp) {
    const DataNode &n1 = array->Evaluate(1);
    const DataNode &n2 = array->Evaluate(2);
    const DataNode &n3 = array->Evaluate(3);
    if (n1.Type() == kDataFloat || n2.Type() == kDataFloat || n3.Type() == kDataFloat) {
        return Clamp<float>(
            n2.LiteralFloat(array), n3.LiteralFloat(array), n1.LiteralFloat(array)
        );
    } else {
        return Clamp<int>(
            n2.LiteralInt(array), n3.LiteralInt(array), n1.LiteralInt(array)
        );
    }
}

DEF_DATA_FUNC(DataSubEq) {
    DataNode ret = DataSub(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x285);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}

DEF_DATA_FUNC(DataClampEq) {
    DataNode ret = DataClamp(array);
    if (array->Type(1) == kDataProperty) {
        MILO_ASSERT(gDataThis, 0x28C);
        gDataThis->SetProperty(array->UncheckedArray(1), ret);
    } else
        *array->Var(1) = ret;
    return ret;
}
