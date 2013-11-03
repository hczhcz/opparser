#ifndef __INC_OPPARSER_HPP__
#define __INC_OPPARSER_HPP__

#include <memory>
#include <vector>
#include <map>
#include <string>
// The namespace of the operator-precedence parser
namespace OPParser {
    using namespace std;

    // Type of the state of the parser
    typedef int State;

    // Type of the precedence level of operators
    typedef int Level;

    // Input data type
    typedef string Input;

    // Input iterator type
    typedef Input::const_iterator InputIter;

    // Initial state
    const State stateInitial = 0;

    class Lexer;
    class Token;
    class Parser;

    // Use pointer instead of reference
    typedef shared_ptr <Lexer> PLexer;
    typedef shared_ptr <Token> PToken;

    // Throw error
    void error(const string &info);

    // Runtime checking like assert
    // If failed, throw error
    void check(const bool condition, const string &info);

    // Lexer particle, recognise token from string
    // Chain-factory, to create token
    class Lexer: public enable_shared_from_this <Lexer>{
    public:
        // If can create this token, return true, increase the offset of input and push token
        // If input can be ignored, return true and increase the offset
        // If nothing can be done, return false and throw the task to next lexer
        virtual bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) = 0;
    };

    // Token, contains token information and final data (after pop from middle stack)
    class Token: public enable_shared_from_this <Token>{
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

        // Show as string (input form)
        // For debug only
        virtual Input show() = 0;
    };

    // The operator-precedence parser
    // Using modified shunting-yard algorithm
    // Must initialize before use
    class Parser {
    protected:
        // Map of lexer chains
        map <State, vector <PLexer> > lexers = {};
    public:
        State state;
        vector <PToken> midStack = {};
        vector <PToken> outStack = {};

        // Initialization
        // Will call reset() here
        virtual void init();

        // Reset
        // Clean up and start parsing
        virtual void reset();

        // Push to middle stack
        void midPush(PToken token);

        // Pop from middle stack
        void midPop();

        // Parse a string
        // Push data to lexers
        virtual void parse(const Input &input);

        // Finish parsing
        // Will call reset() here
        virtual void finish(vector <PToken> &result);
    };
}

#endif
