#include <iostream>
#include "opcalcrepl.hpp"

int main() {
    using namespace std;
    using namespace OPParser;

    CalcRepl calc;
    calc.run(L"q");
}
