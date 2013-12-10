#ifndef __INC_CALCNEAR_HPP__
#define __INC_CALCNEAR_HPP__

#include <map>
#include <string>

// Get near value
#define fnear(x) ((x) + 65536 - 65536)

namespace OPParser {
    using namespace std;

    // Output data type
    typedef string Output;

    // Near value type
    typedef float CalcNearData;

    // Known value table, like {0.5, "1 / 2"}
    extern const map <CalcNearData, Output> NearValue;
}

#endif
