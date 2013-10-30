#include "opparser.hpp"

namespace OPParser {
    void error(const string info) {
        throw runtime_error(info);
    }

    void check(const bool condition, const string info) {
        if (!condition) {
            error(info);
        }
    }

    // An abstract token to pop everything from middle stack
    class FinToken: public Token {
    public:
        Level levelLeft() {
            return numeric_limits <Level>::min();
        }
        Level levelRight() {
            error("Input already finished");
            // Never reach
            return numeric_limits <Level>::max();
        }
        void onPush(Parser &parser) {
            //
        }
        void onPop(Parser &parser) {
            //
        }
    };

    void Parser::reset() {
        state = stateInitial;
        midStack.clear();
        outStack.clear();
    }

    void Parser::midPush(PToken &token) {
        token->onPush(*this);

        if (!midStack.empty()) {
            while (1) {
                // Pop all lower-level tokens
                if (midStack.back()->levelRight() > token->levelLeft()) {
                    midPop();
                    continue;
                }
                if (midStack.back()->levelRight() < token->levelLeft()) {
                    break;
                }
                // Wrong
                error("Token collision");
            }
        }

        midStack.push_back(token);
    }

    // Pop from middle stack
    void Parser::midPop() {
        midStack.back()->onPop(*this);
        // outstack.push_back(midStack.back());
        midStack.pop_back();
    }

    void Parser::parse(const Input &input) {
        InputIter now = input.begin();
        const InputIter end = input.end();
        while (now != end) {
            vector <PLexer> &nowlexers = lexers[state];

            // Scan the lexer chain
            vector <PLexer>::iterator iter = nowlexers.begin();
            while (1) {
                check(iter != nowlexers.end(), "Unknown token");

                if ((*iter)->tryGetToken(now, end, *this)) {
                    break;
                }

                ++iter;
            }
        }
    }

    void Parser::finish(vector <PToken> &result, bool allowContinue) {
        // check(state == stateInitial, "Wrong finalize state");

        // Clear middle stack
        FinToken _finTokenInstance;
        PToken finToken = &_finTokenInstance;
        midPush(finToken);

        // assert(midStack.back() == finToken);
        if (midStack.size() != 1) {
            if (allowContinue) {
                midPop();
            } else {
                error("Input not completed");
            }
        }

        // Return outstack as result
        result = outStack;

        reset();
    }
}
