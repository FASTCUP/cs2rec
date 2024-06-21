#pragma once
#include <cstddef>
#include <cstdlib>

// NOTE(Cade):
//   This is a dummy file which multiple external files will include.
//   It also contains numerous hacks to minimize patches in the advancedfx repo(s).
//
//   Most of these functions are "unused"; The templates using them aren't instantiated.
//   I suspect MSVC didn't link them as a result. But GCC still tries.

