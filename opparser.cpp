#include <stdexcept>
#include <limits>
#include "opparser.hpp"

namespace OPParser {
    void error(const string &info) {
        throw opparser_error(info);
    }

    void check(const bool condition, const string &info) {
        if (!condition) {
            error(info);
        }
    }

    // An abstract token to pop everything from middle stack
    // For stack allocation only
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
            // Nothing
        }

        void onPop(Parser &parser) {
            check(parser.midStack.empty(), "Input not completed");
        }
    };

    void Parser::reset() {
        state = stateInitial;
        midStack.clear();
        outStack.clear();
    }

    void Parser::init() {
        reset();
        lexers.clear();
        addFirstLexers();
        addLastLexers();
    }

    void Parser::midPush(PToken token) {
        token->onPush(*this);

        while (!midStack.empty()) {
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

        midStack.push_back(token);
    }

    // Pop from middle stack
    void Parser::midPop() {
        check(!midStack.empty(), "No token to pop");
        PToken token(midStack.back());
        midStack.pop_back();
        token->onPop(*this);
    }

    void Parser::parse(const Input &input) {
        InputIter now = input.begin();
        const InputIter end = input.end();

        // Scan input
        while (now != end) {
            vector <PLexer> &nowlexers = lexers[state];

            // Scan the lexers chain
            vector <PLexer>::iterator iter = nowlexers.begin();
            while (1) {
                check(iter != nowlexers.end(), "Unknown token");

                // Try lexers
                if ((*iter)->tryGetToken(now, end, *this)) {
                    break;
                }

                ++iter;
            }
        }
    }

    void Parser::finish(vector <PToken> &result) {
        // check(state == stateInitial, "Wrong finalize state");

        // Clear middle stack
        // Use a FinToken to pop everything
        PToken token(new FinToken());
        midPush(token);
        midPop();

        // Return outstack as result
        result = outStack;

        reset();
    }
}
