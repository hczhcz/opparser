#ifndef __INC_CALC_HPP__
#define __INC_CALC_HPP__

#include "opcalcrule.hpp"

namespace OPParser {
    // Calculator, to calculate arithmetic expressions
    // A simple example of implementing of the parser
    class Calc: public Parser {
    protected:
        // Push math tokens' lexers to the parser
        void addFirstLexers();

        // Push blank and implicit multiplication
        void addLastLexers();
    public:
        // Finish parsing and return result
        CalcData finishByData();
    };
}

#endif
