#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cmath>
#include "opcalc.hpp"
#include "opcalcrule.hpp"

namespace OPParser {
    enum BiOperType {otAdd, otSub, otMul, otDiv, otMod, otPwr};
    enum MonoOperType {otPos, otNeg, otFac};

    class NumToken;
    class BiToken;
    class MonoToken;
    class LeftToken;
    class RightToken;
    typedef shared_ptr <NumToken  > PNumToken;
    typedef shared_ptr <BiToken   > PBiToken;
    typedef shared_ptr <MonoToken > PMonoToken;
    typedef shared_ptr <LeftToken > PLeftToken;
    typedef shared_ptr <RightToken> PRightToken;

    // Tokens

    // Number
    class NumToken: public Token {
    protected:
        CalcData value = 0;
    public:
        friend class Calc;
        friend class BiToken;
        friend class MonoToken;

        NumToken(CalcData toValue) {
            value = toValue;
        }

        Level levelLeft() {
            return levelConst;
        }

        Level levelRight() {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            parser.outStack.push_back(shared_from_this());
        }
    };

    // Bi-operators
    class BiToken: public Token {
    protected:
        BiOperType type;
    public:
        BiToken(BiOperType toType) {
            type = toType;
        }

        Level levelLeft() {
            const Level toMap[] = {levelAddSubL, levelAddSubL, levelMulDivL, levelMulDivL, levelMulDivL, levelPwrL};
            return toMap[type];
        }

        Level levelRight() {
            const Level toMap[] = {levelAddSubR, levelAddSubR, levelMulDivR, levelMulDivR, levelMulDivR, levelPwrR};
            return toMap[type];
        }

        void onPush(Parser &parser) {
            parser.state = stateNum;
        }

        void onPop(Parser &parser) {
            check(parser.outStack.size() >= 2, "No operand");

            // Cast the tokens
            // Tokens in outStack should be numbers
            PNumToken tRight = dynamic_pointer_cast <NumToken> (
                parser.outStack.back()
            );
            parser.outStack.pop_back();
            PNumToken tLeft = dynamic_pointer_cast <NumToken> (
                parser.outStack.back()
            );

            check(tRight != nullptr && tLeft != nullptr, "Unknown operand");

            // Do calculation
            switch (type) {
            case otAdd:
                tLeft->value += tRight->value;
                break;
            case otSub:
                tLeft->value -= tRight->value;
                break;
            case otMul:
                tLeft->value *= tRight->value;
                break;
            case otDiv:
                tLeft->value /= tRight->value;
                break;
            case otMod:
                tLeft->value -= int(tLeft->value / tRight->value) * tRight->value;
                break;
            case otPwr:
                tLeft->value = pow(tLeft->value, tRight->value);
                break;
            }
        }
    };

    // Mono-operators
    class MonoToken: public Token {
    protected:
        MonoOperType type;
    public:
        MonoToken(MonoOperType toType) {
            type = toType;
        }

        Level levelLeft() {
            const Level toMap[] = {levelConst, levelConst, levelFacL};
            return toMap[type];
        }

        Level levelRight() {
            const Level toMap[] = {levelPosNegR, levelPosNegR, levelConst};
            return toMap[type];
        }

        void onPush(Parser &parser) {
            const State toMap[] = {stateNum, stateNum, stateOper};
            parser.state = toMap[type];
        }

        void onPop(Parser &parser) {
            check(!parser.outStack.empty(), "No operand");

            // Cast the token
            // Tokens in outStack should be numbers
            PNumToken tTarget = dynamic_pointer_cast <NumToken> (
                parser.outStack.back()
            );
            check(tTarget != nullptr, "Unknown operand");

            // Do calculation
            switch (type) {
            case otPos:
                // tTarget->value = +tTarget->value;
                break;
            case otNeg:
                tTarget->value = -tTarget->value;
                break;
            case otFac:
                // x! == gamma(x + 1)
                CalcData &x = tTarget->value;
                if (x < 0) {
                    x = -NAN;
                } else if (x < 0.7) {
                    // Taylor Series
                    const CalcData x1  = x;         // m0 d1
                    const CalcData x2  = x1 * x1;   // m1 d2
                    const CalcData x3  = x2 * x1;   // m2 d3
                    const CalcData x4  = x2 * x2;   // m2 d3
                    const CalcData x5  = x4 * x1;   // m3 d4
                    const CalcData x6  = x4 * x2;   // m3 d4
                    const CalcData x7  = x4 * x3;   // m3 d4
                    const CalcData x8  = x4 * x4;   // m3 d4
                    const CalcData x9  = x8 * x1;   // m4 d5
                    const CalcData x10 = x8 * x2;   // m4 d5
                    const CalcData x11 = x8 * x3;   // m4 d5
                    x =
                        + 1.00000000000000000000       - 0.57721566490153286061 * x1
                        + 0.98905599532797255540 * x2  - 0.90747907608088628902 * x3
                        + 0.98172808683440018734 * x4  - 0.98199506890314520210 * x5
                        + 0.99314911462127619315 * x6  - 0.99600176044243153397 * x7
                        + 0.99810569378312892198 * x8  - 0.99902526762195486779 * x9
                        + 0.85077168542087678    * x10 - 0.37664551140085667    * x11
                    ;
                } else {
                    // Stirling Series
                    const CalcData x1  = 1 / x;     // m0 d1
                    const CalcData x2  = x1 * x1;   // m1 d2
                    const CalcData x3  = x2 * x1;   // m2 d3
                    const CalcData x4  = x2 * x2;   // m2 d3
                    const CalcData x5  = x4 * x1;   // m3 d4
                    const CalcData x6  = x4 * x2;   // m3 d4
                    const CalcData x7  = x4 * x3;   // m3 d4
                    const CalcData x8  = x4 * x4;   // m3 d4
                    const CalcData x9  = x8 * x1;   // m4 d5
                    const CalcData x10 = x8 * x2;   // m4 d5
                    const CalcData x11 = x8 * x3;   // m4 d5
                    x = (
                        + 0.00016579876471393590    * x11 - 0.0006721951190716635     * x10
                        + 0.00083949872067208727999 * x9  - 0.000051717909082605921934 * x8
                        - 0.00059216643735369388286 * x7  + 0.000069728137583658577743 * x6
                        + 0.00078403922172006662747 * x5  - 0.00022947209362139917695  * x4
                        - 0.0026813271604938271605  * x3  + 0.0034722222222222222222   * x2
                        + 0.083333333333333333333   * x1  + 1.0000000000000000000
                    ) * sqrt(2.0 * M_PI) * pow(x, x + 0.5) * exp(-x);
                }
                break;
            }
        }
    };

    // Left bracket
    class LeftToken: public Token {
    public:
        Level levelLeft() {
            return levelConst;
        }

        Level levelRight() {
            return levelAcceptAll;
        }

        void onPush(Parser &parser) {
            parser.state = stateNum;
        }

        void onPop(Parser &parser) {
        }
    };

    // Right bracket
    class RightToken: public Token {
    public:
        Level levelLeft() {
            return levelFlushAll;
        }

        Level levelRight() {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            check(!parser.midStack.empty(), "No left bracket");

            // Cast the token to left bracket, then delete it
            PLeftToken tLB = dynamic_pointer_cast <LeftToken> (
                parser.midStack.back()
            );
            check(tLB != nullptr, "Bad left bracket");

            parser.midPop();
        }
    };

    // Lexers

    // Numbers
    class NumLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            char buffer[256];
            unsigned index = 0;

            // Read number to buffer
            for (; (index < 255) && (now != end); ++index) {
                switch (*now) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '.':
                    buffer[index] = *now;
                    ++now;
                    continue;
                }
                break;
            }

            if (index == 0) {
                // If not accepted
                return 0;
            } else {
                // If accepted
                // Generate token

                buffer[index] = 0;

                char *endPtr;
                CalcData number = strtod(buffer, &endPtr);

                check(*endPtr == 0, "Wrong format of number");

                PToken token(new NumToken(number));
                parser.midPush(token);
                return 1;
            }
        }
    };

    // Operators appear after number
    class AfterNumLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            PToken token(nullptr);

            // Cast and recognise token
            switch (*now) {
            case '+':
                token = PToken(new BiToken(otAdd));
                break;
            case '-':
                token = PToken(new BiToken(otSub));
                break;
            case '*':
                token = PToken(new BiToken(otMul));
                break;
            case '/':
                token = PToken(new BiToken(otDiv));
                break;
            case '%':
                token = PToken(new BiToken(otMod));
                break;
            case '^':
                token = PToken(new BiToken(otPwr));
                break;
            case '!':
                token = PToken(new MonoToken(otFac));
                break;
            }

            if (token != nullptr) {
                // Accepted
                ++now;
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Operators appear without number before
    class NoNumLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            PToken token(nullptr);

            // Cast and recognise token
            switch (*now) {
            case '+':
                token = PToken(new MonoToken(otPos));
                break;
            case '-':
                token = PToken(new MonoToken(otNeg));
                break;
            }

            if (token != nullptr) {
                // Accepted
                ++now;
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Left bracket
    class LeftLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == '(') {
                // Accepted
                ++now;
                PToken token(new LeftToken());
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Right bracket
    class RightLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == ')') {
                // Accepted
                ++now;
                PToken token(new RightToken());
                parser.midPush(token);
                return 1;
            } else {
                return 0;
            }
        }
    };

    // Skip blank, like white spaces
    class BlankLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            switch (*now) {
            case 0:
            case '\t':
            case '\n':
            case '\r':
            case ' ':
                ++now;
                return 1;
            default:
                return 0;
            }
        }
    };

    void Calc::init() {
        Parser::init();

        {
            PLexer lexer(new NumLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new AfterNumLexer());
            lexers[stateOper].push_back(lexer);
        }
        {
            PLexer lexer(new NoNumLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new LeftLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new RightLexer());
            lexers[stateOper].push_back(lexer);
        }
        {
            PLexer lexer(new BlankLexer());
            lexers[stateNum].push_back(lexer);
            lexers[stateOper].push_back(lexer);
        }
    }

    CalcData Calc::finishByData() {
        vector <PToken> result;
        finish(result);

        check(result.size() == 1, "Bad result");

        // Get result
        PNumToken tResult = dynamic_pointer_cast <NumToken> (
            result.back()
        );

        check(tResult != nullptr, "Bad result");

        return tResult->value;
    }
}
