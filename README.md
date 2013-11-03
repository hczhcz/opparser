A simple operator-precedence parser
===

A parser (in `opparser.cpp`) and a demo (a calculator, see `opcalc.cpp` and `opcalcrepl.cpp`).

For fun.

Build the calculator
---
1. Compile (clang++ for example)

	clang++ -std=c++11 opparser.cpp
	clang++ -std=c++11 opcalc.cpp
	clang++ -std=c++11 opcalcrepl.cpp
	clang++ -std=c++11 project.cpp

2. Link

	clang++ *.o -o calc

3. Run

	./calc

4. Have fun

	> 1+1
	  = 2
	> 2.5!
	  = 3.32335
	> 1/(37%3)
	  = 1
	> 3^2;3^3;3^4
	  = 9
	  = 27
	  = 81
	> 1/0
	  = inf
	> +
	  # No operand
	> aaa
	  # Unknown token
	> --1  -   3 ;  ; ;;;
	  = -2
	> q

Implement your own language
---
1. Write a new parser class

	using namespace OPParser;
	class SomeParser: public Parser;

2. Write tokens

	class SomeToken: public Token {
	public:
	    Level levelLeft() {
		return ?;
	    }

	    Level levelRight() {
		return ?;
	    }

	    void onPush(Parser &parser) {
		parser.state = ?;
	    }

	    void onPop(Parser &parser) {
		parser.outStack.back()->?;
		parser.outStack.pop_back();

		// Do something

		parser.outStack.push_back(?);
	    }

	    Input show() {
		return "?";
	    }
	};

3. Write lexers

	class SomeLexer: public Lexer {
	public:
	    bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
		if (*now == '?') {
		    ++now;
		    PToken token(new SomeToken());
		    parser.midPush(token);
		    return 1;
		} else {
		    return 0;
		}
	    }
	};

4. Add lexers to the parser

	void SomeParser::init() {
	    Parser::init();

	    PLexer lexer(new SomeLexer());
	    lexers[stateNum].push_back(lexer);

	    // More...
	}

5. Use the parser

	SomeParser parser;
	parser.init();
	parser.parse(?);

	vector <PToken> result;
	parser.finish(result);

	// Do something

