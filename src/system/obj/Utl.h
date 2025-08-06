#pragma once
#include "obj/Dir.h"
#include "utl/Symbol.h"
#include "obj/Data.h"

/** Determine whether or not child is a subclass of parent.
 * @param [in] child The child class's name, in Symbol form.
 * @param [in] parent The potential parent class's name, in Symbol form.
 * @returns True if child is a subclass of parent, false if not.
 */
bool IsASubclass(Symbol child, Symbol parent);

const char *PrintPropertyPath(DataArray *);

void ReserveToFit(ObjectDir *, ObjectDir *, int);
