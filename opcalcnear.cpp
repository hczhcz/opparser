#include "opcalcnear.hpp"

namespace OPParser {
    const map <CalcNearData, Output> NearValue = {
        #include "nearnum.inc"
    };
}
