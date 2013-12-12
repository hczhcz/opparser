#include <cstdlib>
#include "opcalc.hpp"

namespace OPParser {
    class NumToken;
    class FuncToken;
    class AssignToken;
    class BiToken;
    class MonoToken;
    class LeftToken;
    class RightToken;
    typedef shared_ptr <NumToken    > PNumToken;
    typedef shared_ptr <FuncToken   > PFuncToken;
    typedef shared_ptr <AssignToken > PAssignToken;
    typedef shared_ptr <BiToken     > PBiToken;
    typedef shared_ptr <MonoToken   > PMonoToken;
    typedef shared_ptr <LeftToken   > PLeftToken;
    typedef shared_ptr <RightToken  > PRightToken;

    // Tokens

    // Number
    class NumToken: public Token {
    protected:
        CalcData value = 0;
    public:
        friend class Calc;
        friend class FuncToken;
        friend class AssignToken;
        friend class BiToken;
        friend class MonoToken;

        NumToken(CalcData toValue): value(toValue) {}

        Level levelLeft() const {
            return levelConst;
        }

        Level levelRight() const {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            parser.outStack.push_back(shared_from_this());
        }
    };

    // Functions
    class FuncToken: public Token {
    protected:
        FuncType type;
    public:
        FuncToken(FuncType toType): type(toType) {}

        Level levelLeft() const {
            return levelConst;
        }

        Level levelRight() const {
            return levelFuncR;
        }

        void onPush(Parser &parser) {
            parser.state = stateNum;
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
            case ftSin:
                tTarget->value = sin(tTarget->value);
                break;
            case ftCos:
                tTarget->value = cos(tTarget->value);
                break;
            case ftTan:
                tTarget->value = tan(tTarget->value);
                break;
            case ftASin:
                tTarget->value = asin(tTarget->value);
                break;
            case ftACos:
                tTarget->value = acos(tTarget->value);
                break;
            case ftATan:
                tTarget->value = atan(tTarget->value);
                break;
            case ftSinH:
                tTarget->value = sinh(tTarget->value);
                break;
            case ftCosH:
                tTarget->value = cosh(tTarget->value);
                break;
            case ftTanH:
                tTarget->value = tanh(tTarget->value);
                break;
            case ftASinH:
                tTarget->value = asinh(tTarget->value);
                break;
            case ftACosH:
                tTarget->value = acosh(tTarget->value);
                break;
            case ftATanH:
                tTarget->value = atanh(tTarget->value);
                break;
            case ftLog:
                tTarget->value = log(tTarget->value);
                break;
            case ftLog10:
                tTarget->value = log10(tTarget->value);
                break;
            case ftLog2:
                tTarget->value = log2(tTarget->value);
                break;
            case ftSqr:
                tTarget->value *= tTarget->value;
                break;
            case ftSqrt:
                tTarget->value = sqrt(tTarget->value);
                break;
            case ftAbs:
                tTarget->value = abs(tTarget->value);
                break;
            case ftSign:
                tTarget->value = int(tTarget->value > 0) - int(tTarget->value < 0);
                break;
            case ftDeg:
                tTarget->value *= (180 / M_PI);
                break;
            case ftRad:
                tTarget->value *= (M_PI / 180);
                break;
            case ftErf:
                tTarget->value = erf(tTarget->value);
                break;
            case ftErfc:
                tTarget->value = erfc(tTarget->value);
                break;
            case ftGamma:
                tTarget->value = tgamma(tTarget->value);
                break;
            case ftLGamma:
                tTarget->value = lgamma(tTarget->value);
                break;
            case ftCeil:
                tTarget->value = ceil(tTarget->value);
                break;
            case ftFloor:
                tTarget->value = floor(tTarget->value);
                break;
            case ftTrunc:
                tTarget->value = trunc(tTarget->value);
                break;
            case ftRound:
                tTarget->value = round(tTarget->value);
                break;
            case ftInt:
                tTarget->value = int(tTarget->value);
                break;
            }
        }
    };

    // Assignation (reference)
    class AssignToken: public Token {
    protected:
        Input name;
    public:
        AssignToken(Input &toName): name(toName) {}

        Level levelLeft() const {
            return levelFlushAll;
        }

        Level levelRight() const {
            return levelConst;
        }

        void onPush(Parser &parser) {
            parser.state = stateOper;
        }

        void onPop(Parser &parser) {
            check(!parser.outStack.empty(), "No operand");

            // Cast the token
            // Tokens in outStack should be numbers
            PNumToken tTarget = dynamic_pointer_cast <NumToken> (
                parser.outStack.back()
            );
            check(tTarget != nullptr, "Unknown operand");

            // Do assignation
            GetConst[name] = tTarget->value;
        }
    };

    // Bi-operators
    class BiToken: public Token {
    protected:
        BiOperType type;
    public:
        BiToken(BiOperType toType): type(toType) {}

        Level levelLeft() const {
            const Level toMap[] = {levelAddSubL, levelAddSubL, levelMulDivL, levelMulDivL, levelMulDivL, levelPwrL};
            return toMap[type];
        }

        Level levelRight() const {
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
        MonoToken(MonoOperType toType): type(toType) {}

        Level levelLeft() const {
            const Level toMap[] = {levelConst, levelConst, levelFacL};
            return toMap[type];
        }

        Level levelRight() const {
            const Level toMap[] = {levelAddSubR, levelAddSubR, levelConst};
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
            case mtPos:
                // tTarget->value = +tTarget->value;
                break;
            case mtNeg:
                tTarget->value = -tTarget->value;
                break;
            case mtFac:
                // x! == gamma(x + 1)
                tTarget->value = tgamma(tTarget->value + 1);
                break;
            }
        }
    };

    // Left bracket
    class LeftToken: public Token {
    public:
        Level levelLeft() const {
            return levelConst;
        }

        Level levelRight() const {
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
        Level levelLeft() const {
            return levelFlushAll;
        }

        Level levelRight() const {
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
            Input buffer = "";

            if ((*now >= '0' && *now <= '9') || *now == '.') {
                // Accepted
            } else {
                // Not a number
                return 0;
            }

            // Read number to buffer
            for (; now != end; ++now) {
                if ((*now >= '0' && *now <= '9') || *now == '.') {
                    buffer += *now;
                } else {
                    break;
                }
            }

            // Generate token

            char *endPtr;
            CalcData number = strtod(buffer.c_str(), &endPtr);

            check(*endPtr == 0, "Wrong format of number");

            PToken token(new NumToken(number));
            parser.midPush(token);
            return 1;
        }
    };

    // Functions and constants
    class NameLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            Input buffer = "";

            if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_') {
                // Accepted
            } else {
                // Not a name
                return 0;
            }

            // Read name to buffer
            for (; now != end; ++now) {
                if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_' || (*now >= '0' && *now <= '9')) {
                    buffer += *now;
                } else {
                    break;
                }
            }

            // Generate token

            PToken token(nullptr);
            if (GetFunc.find(buffer) != GetFunc.end()) {
                token = PToken(new FuncToken(GetFunc[buffer]));
            } else if (GetConst.find(buffer) != GetConst.end()) {
                token = PToken(new NumToken(GetConst[buffer]));
            } else {
                error("Unknown function or constant");
            }

            parser.midPush(token);
            return 1;
        }
    };

    // Constants reference (for assignation)
    class NameRefLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            Input buffer = "";

            if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_') {
                // Accepted
            } else {
                // Not a name
                return 0;
            }

            // Read name to buffer
            for (; now != end; ++now) {
                if ((*now >= 'A' && *now <= 'Z') || (*now >= 'a' && *now <= 'z') || *now == '_' || (*now >= '0' && *now <= '9')) {
                    buffer += *now;
                } else {
                    break;
                }
            }

            // Generate token

            PToken token(new AssignToken(buffer));

            check(GetFunc.find(buffer) == GetFunc.end(), "Can not assign to a function");

            parser.midPush(token);
            return 1;
        }
    };

    // Assignations
    // 1 -> x
    class AssignLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == '-' && now + 1 != end && *(now + 1) == '>') {
                // Accepted
                now += 2;
                parser.state = stateAssign;
                return 1;
            } else {
                // Not accepted
                return 0;
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
                token = PToken(new MonoToken(mtFac));
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
                token = PToken(new MonoToken(mtPos));
                break;
            case '-':
                token = PToken(new MonoToken(mtNeg));
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

    // Implicit multiplication
    // Like "3 pi = 3 * pi"
    // Should be the last one in the lexers chain
    class ImplicitMulLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            PToken token(new BiToken(otMul));
            parser.midPush(token);
            return 1;
        }
    };

    void Calc::addFirstLexers() {
        {
            PLexer lexer(new NumLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new NameLexer());
            lexers[stateNum].push_back(lexer);
        }
        {
            PLexer lexer(new NameRefLexer());
            lexers[stateAssign].push_back(lexer);
        }
        {
            PLexer lexer(new AssignLexer());
            lexers[stateOper].push_back(lexer);
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
    }

    void Calc::addLastLexers() {
        {
            PLexer lexer(new BlankLexer());
            lexers[stateNum].push_back(lexer);
            lexers[stateOper].push_back(lexer);
            lexers[stateAssign].push_back(lexer);
        }
        {
            PLexer lexer(new ImplicitMulLexer());
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

        GetConst["ans"] = tResult->value;

        return tResult->value;
    }
}
