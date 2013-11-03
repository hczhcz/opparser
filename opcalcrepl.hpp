#ifndef __INC_CALCREPL_HPP__
#define __INC_CALCREPL_HPP__

#include <iostream>
#include "opcalc.hpp"
#include "opcalcrule.hpp"

namespace OPParser {
    // Calculator with REPL
    class CalcRepl: public Calc {
    protected:
        istream *in = &cin;
        ostream *out = &cout;

        string exitSign = "q";
        bool running = 0;
    public:
        // Initialization
        // Push ";" lexer
        void init();

        // Read from input stream
        void read();

        // Run and write result to output stream
        void write();

        // Run REPL interpreter with input and output stream
        // Input exit sign to exit
        void run(Input exitSign);
    };
}

#endif
