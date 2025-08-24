#include "ui/UIListProvider.h"
#include "obj/Data.h"
#include "obj/Object.h"

BEGIN_CUSTOM_HANDLERS(UIListProvider)
    HANDLE_EXPR(num_data, NumData())
    HANDLE_EXPR(data_index, DataIndex(_msg->Sym(2)))
    HANDLE_EXPR(data_symbol, DataSymbol(_msg->Int(2)))
END_CUSTOM_HANDLERS
