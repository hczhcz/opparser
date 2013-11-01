#ifndef __INC_CALC_HPP__
#define __INC_CALC_HPP__

#include "opparser.hpp"

namespace OPParser {
    // Calculator, to calculate arithmetic expressions
    // A simple example of implementing of the parser
    class Calc: public Parser {
    public:
        // Initialization
        // Push lexers to the parser
        void init();
    };
}

#endif
