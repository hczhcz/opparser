#ifndef __INC_CALCRULE_HPP__
#define __INC_CALCRULE_HPP__

#define _USE_MATH_DEFINES
#include <cmath>
#include "opparser.hpp"

namespace OPParser {
    // Type of data in the calculator
    typedef double CalcData;

    // Precedence levels, from 0 to 4095
    const Level levelConst = 4095;
    const Level levelAcceptAll = 0;
    const Level levelFlushAll = 1;
    const Level levelAddSubL = 255;
    const Level levelAddSubR = 256;
    const Level levelMulDivL = 511;
    const Level levelMulDivR = 512;
    const Level levelIMulL = 639;
    const Level levelIMulR = 640;
    const Level levelPwrL = 769;
    const Level levelPwrR = 768;
    const Level levelFacL = 1023;
    const Level levelFuncR = 1280;

    // States of the parser
    const State stateNum = stateInitial;
    const State stateOper = stateInitial + 1;
    const State stateAssign = stateInitial + 2;

    // Bi-operators
    enum BiOperType {otAdd, otSub, otMul, otIMul, otDiv, otMod, otPwr};

    // Mono-operators
    enum MonoOperType {mtPos, mtNeg, mtFac};

    // Functions
    enum FuncType {ftSin, ftCos, ftTan, ftASin, ftACos, ftATan,
                   ftSinH, ftCosH, ftTanH, ftASinH, ftACosH, ftATanH,
                   ftLog, ftLog10, ftLog2, ftSqr, ftSqrt, ftAbs, ftSign,
                   ftDeg, ftRad, ftErf, ftErfc, ftGamma, ftLGamma,
                   ftCeil, ftFloor, ftTrunc, ftRound, ftInt};

    // Function name-type map
    extern map <Input, FuncType> GetFunc;

    // Const name-value map
    extern map <Input, CalcData> GetConst;
}

#endif
