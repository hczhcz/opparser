#ifndef __INC_CALCNEAR_HPP__
#define __INC_CALCNEAR_HPP__

#include "opparser.hpp"
// Get near value
#define fnear(x) ((x) + 256 - 256)

namespace OPParser {
    // Near value type
    typedef float CalcNearData;

    // Known value table, like {0.5, "1 / 2"}
    extern const map <CalcNearData, Input> NearValue;
}

#endif
