#include "ui/CheatProvider.h"
#include "obj/DataUtl.h"
#include "os/Debug.h"
#include "os/System.h"
// #include "ui/UIListLabel.h"
#include "utl/Cheats.h"

CheatProvider *CheatProvider::sInstance;

CheatProvider::CheatProvider() : mFilterIdx(0) {
    SetName("cheat_provider", ObjectDir::Main());
    mFilters.push_back("all");
    DataArray *cfg = SystemConfig("quick_cheats");
    for (int i = 1; i < cfg->Size(); i++) {
        DataArray *arr = cfg->Array(i);
        Symbol cheatTypeSym = arr->Sym(0);
        const char *strtext = 0;
        if (cheatTypeSym == "keyboard")
            strtext = "KEYBOARD CHEATS";
        else if (cheatTypeSym == "right")
            strtext = "RIGHT CHEATS (R1 + R2)";
        else if (cheatTypeSym == "left")
            strtext = "LEFT CHEATS (L1 + L2)";
        mCheats.push_back(Cheat(strtext));
        for (int j = 1; j < arr->Size(); j++) {
            DataArray *arr2 = arr->Array(j);
            DataType nType = arr2->Node(0).Type();
            String theKeyStr;
            if (nType == kDataString || nType == kDataSymbol) {
                theKeyStr = arr2->Str(0);
            } else {
                const char *cheatStrStart = cheatTypeSym == "keyboard" ? "KB_" : "kPad_";
                theKeyStr = DataGetMacroByInt(arr2->Int(0), cheatStrStart).Str()
                    + strlen(cheatStrStart);
            }
            String theConcattedStr;
            int theNodeIdx = 1;
            while (arr2->Type(theNodeIdx) == kDataSymbol) {
                theKeyStr = String(arr2->Sym(theNodeIdx)) + String(" ") + theKeyStr;
                theNodeIdx++;
            }
            theConcattedStr = arr2->Str(theNodeIdx);
            mCheats.push_back(Cheat(theKeyStr, theConcattedStr, arr2));
            static Symbol filters("filters");
            DataArray *filterArr = arr2->FindArray(filters, false);
            if (filterArr) {
                for (int k = 1; k < filterArr->Size(); k++) {
                    Symbol curFilt = filterArr->Sym(k);
                    if (std::find(mFilters.begin(), mFilters.end(), curFilt)
                        == mFilters.end()) {
                        mFilters.push_back(curFilt);
                    }
                }
            }
        }
        if (i < cfg->Size() - 1) {
            mCheats.push_back(gNullStr);
        }
    }
    ApplyFilter();
}
