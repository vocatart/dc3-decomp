#pragma once
#include "obj/Data.h"
#include "obj/Object.h"
#include <map>

extern std::map<Symbol, DataArray *> gMacroTable;

/** The directory gDataThis is stored in. */
extern class ObjectDir *gDataDir;

/** The Hmx::Object represented by the `$this` variable. */
extern Hmx::Object *gDataThis;

/** Returns gDataThis. */
Hmx::Object *DataThis();
Hmx::Object *DataSetThis(Hmx::Object *o);

void DataInit();
void DataTerminate();
void DataMacroWarning(bool);

bool DataUpdateArray(DataArray *, DataArray *);
