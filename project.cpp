#include <iostream>
#include "opcalc.hpp"

using namespace std;
using namespace OPParser;

int main() {
    Calc calc;

    Input test("1+1");

    calc.init();
    calc.parse(test);
    cout<<calc.finishByData();
}
