#pragma once

#include <vector>
#include "pyxieTypes.h"

namespace pyxie
{
	bool PYXIE_EXPORT ContractDatabase(const char* directory, const char* outDatabase, const char* excludeType=nullptr);
	bool PYXIE_EXPORT ExpandDatabase(const char* inDatabase, const char* outDirectory);
}