#include "opcalcrule.hpp"

namespace OPParser {
    map <Input, FuncType> GetFunc = {
        {"sin", ftSin}, {"cos", ftCos}, {"tan", ftTan}, {"asin", ftASin}, {"acos", ftACos}, {"atan", ftATan},
        {"sinh", ftSinH}, {"cosh", ftCosH}, {"tanh", ftTanH}, {"asinh", ftASinH}, {"acosh", ftACosH}, {"atanh", ftATanH},
        {"log", ftLog}, {"log10", ftLog10}, {"log2", ftLog2}, {"sqr", ftSqr}, {"sqrt", ftSqrt}, {"abs", ftAbs}, {"sign", ftSign},
        {"deg", ftDeg}, {"rad", ftRad}, {"erf", ftErf}, {"erfc", ftErfc}, {"gamma", ftGamma}, {"lgamma", ftLGamma},
        {"ceil", ftCeil}, {"floor", ftFloor}, {"trunc", ftTrunc}, {"round", ftRound}, {"int", ftInt}
    };

    map <Input, CalcData> GetConst = {
        {"pi", M_PI}, {"e", M_E}, {"tau", 2 * M_PI}, {"phi", (sqrt(5) - 1) / 2}, {"inf", INFINITY}, {"nan", NAN}, {"ans", 0}
    };
}
