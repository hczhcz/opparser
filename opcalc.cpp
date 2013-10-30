#define _USE_MATH_DEFINES
#include <cmath>
#include "opcalc.hpp"
#include "opcalcrule.hpp"

namespace OPParser {
    typedef double CalcData;
    enum BiOperType {otAdd, otSub, otMul, otDiv, otMod, otPwr};
    enum MonoOperType {otPos, otNeg, otFac};

    class NumToken;
    class BiToken;
    class MonoToken;
    class LeftToken;
    class RightToken;
    typedef NumToken   *PNumToken;
    typedef BiToken    *PBiToken;
    typedef MonoToken  *PMonoToken;
    typedef LeftToken  *PLeftToken;
    typedef RightToken *PRightToken;

    // Tokens

    // Number
    class NumToken: public Token {
    public:
        CalcData value;

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
            parser.outStack.push_back(this);
        }
    };

    // Bi-operators
    class BiToken: public Token {
    public:
        BiOperType type;

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

            PNumToken tRight = dynamic_cast <PNumToken>(
                parser.outStack.back()
            );
            parser.outStack.pop_back();
            PNumToken tLeft = dynamic_cast <PNumToken>(
                parser.outStack.back()
            );
            check(tRight != 0 && tLeft != 0, "Unknown operand");

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
            case otPwr:
                tLeft->value = pow(tLeft->value, tRight->value);
                break;
            }

            delete tRight;
            delete this;
        }
    };

    // Mono-operators
    class MonoToken: public Token {
    public:
        MonoOperType type;

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

            PNumToken tTarget = dynamic_cast <PNumToken>(
                parser.outStack.back()
            );
            check(tTarget != 0, "Unknown operand");

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

            delete this;
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
            delete this;
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

            PLeftToken tLB = dynamic_cast <PLeftToken>(
                parser.midStack.back()
            );
            check(tLB != 0, "Bad left bracket");

            parser.midPop();
            delete this;
        }
    };

    void Calc::init() {
        lexers.clear();
        // lexers[0].push_back();
    }
}
