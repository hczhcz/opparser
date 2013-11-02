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

        Input show() {
            return "<num>";
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
            const Level toMap[] = {levelAddSubL, levelAddSubL, levelMulDivL, levelMulDivL, levelPwrL};
            return toMap[type];
        }

        Level levelRight() {
            const Level toMap[] = {levelAddSubR, levelAddSubR, levelMulDivR, levelMulDivR, levelPwrR};
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

        Input show() {
            switch (type) {
            case otAdd:
                return "+";
            case otSub:
                return "-";
            case otMul:
                return "*";
            case otDiv:
                return "/";
            case otMod:
                return "%";
            case otPwr:
                return "^";
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
            check(parser.outStack.size() >= 2, "No operand");

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
                if (x == 0) {
                    x = 1;
                } else{
                    CalcData rx = 1 / x;
                    CalcData rxsqr = rx * rx;
                    // Stirling approximation (series)
                    x = sqrt(2 * M_PI * x) * pow(x, x) * (1 + (1 / 12) * rx + (1 / 288) * rxsqr + (-139 / 51840) * rx * rxsqr) / exp(x);
                }
                break;
            }
        }

        Input show() {
            switch (type) {
            case otPos:
                return "+";
            case otNeg:
                return "-";
            case otFac:
                return "!";
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

        Input show() {
            return "(";
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

        Input show() {
            return ")";
        }
    };

    // Lexers

    // Numbers
    class NumLexer: public Lexer {
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            char buffer[256];
            unsigned index = 0;

            // Read number to buffer
            for (; index < 255; ++index) {
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

    // Operators appear after numbers
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

    class NoNumLexer: public Lexer {
        //
    };

    class LeftLexer: public Lexer {
        //
    };

    class RightLexer: public Lexer {
        //
    };

    class BlankLexer: public Lexer {
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

    class FinLexer: public Lexer {
        //
    };

    void Calc::init() {
        lexers.clear();

        PLexer numLexer(new NumLexer());
        lexers[stateNum].push_back(numLexer);

        PLexer afterNumLexer(new AfterNumLexer());
        lexers[stateOper].push_back(afterNumLexer);

        PLexer blankLexer(new BlankLexer());
        lexers[stateNum].push_back(blankLexer);
        lexers[stateOper].push_back(blankLexer);

        reset();
    }

    CalcData Calc::finishByData() {
        vector <PToken> result;
        finish(result);

        check(result.size() == 1, "Bad result");

        PNumToken tResult = dynamic_pointer_cast <NumToken> (
            result.back()
        );

        check(tResult != nullptr, "Bad result");

        return tResult->value;
    }
}
