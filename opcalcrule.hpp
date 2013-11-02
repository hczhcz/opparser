#ifndef __INC_CALCRULE_HPP__
#define __INC_CALCRULE_HPP__

#include "opparser.hpp"

namespace OPParser {
    // 65535 "(" 0
    // 1 ")" 65535
    // 65535 "1" 65535
    // 255 "+" 256
    // 511 "*" 512
    // 767 "^" 768
    // 1023 "!" 65535

    const Level levelConst = 65535;
    const Level levelAcceptAll = 0;
    const Level levelFlushAll = 1;
    const Level levelAddSubL = 255;
    const Level levelAddSubR = 256;
    const Level levelMulDivL = 511;
    const Level levelMulDivR = 512;
    const Level levelPwrL = 769;
    const Level levelPwrR = 768;
    const Level levelFacL = 1023;
    const Level levelPosNegR = 1280;

    const State stateNum = stateInitial;
    const State stateOper = stateInitial + 1;
}

#endif
