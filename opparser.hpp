#ifndef __INC_OPPARSER_HPP__
#define __INC_OPPARSER_HPP__

#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <limits>

namespace OPParser {
    using namespace std;

    typedef int State;
    typedef int Level;
    typedef string Input;
    typedef Input::const_iterator InputIter;

    const State stateInitial = 0;

    class Lexer;
    class Token;
    class Parser;

    typedef Lexer *PLexer;
    typedef Token *PToken;

    // Throw error
    void error(const string info);

    // Runtime checking like assert
    // If failed, throw error
    void check(const bool condition, const string info);

    // Lexer particle, recognise token from string
    // Chain-factory, to create token
    class Lexer {
    public:
        // If can create this token, return true, increase the offset of input and push token
        // If input can be ignored, return true and increase the offset
        // If nothing can be done, return false and throw the task to next lexer
        virtual bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) = 0;
    };

    // Token, contains token information and final data (after pop from middle stack)
    class Token {
    public:
        // Precedence levels
        virtual Level levelLeft() = 0;
        virtual Level levelRight() = 0;

        // Push to middle stack
        // To change state of lexers
        virtual void onPush(Parser &parser) = 0;

        // Pop from middle stack
        // To build final data (using data from output stack)
        // And to push data to output stack
        virtual void onPop(Parser &parser) = 0;
    };

    // The operator-precedence parser
    // Using modified shunting-yard algorithm
    class Parser {
    protected:
        // Map of lexer chains
        map <State, vector <PLexer> > lexers;

        vector <PToken> midStack;
        // vector <PToken> outStack;
    public:
        State state;
        vector <PToken> outStack;

        // Reset (start parsing)
        void reset();

        // Push to middle stack
        void midPush(PToken &token);

        // Pop from middle stack
        void midPop();

        // Parse a string
        void parse(const Input &input);

        // Finish parsing
        void finish(vector <PToken> &result, bool allowContinue);
    };
}

#endif
