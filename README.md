A simple operator-precedence parser
===

A parser (in `opparser.cpp`) and a demo (a calculator, see `opcalc.cpp` and `opcalcrepl.cpp`).

For fun.

Build the calculator
---

Compile (clang++ for example)

	clang++ -std=c++11 opparser.cpp
	clang++ -std=c++11 opcalc.cpp
	clang++ -std=c++11 opcalcrepl.cpp
	clang++ -std=c++11 project.cpp

Link

	clang++ *.o -o calc

Run

	./calc

Have fun

	> 1+1
	  = 2
	> phi
	  = 0.618034
	> e^-1
	  = 0.367879
	> 3^2;3^3;3^4
	  = 9
	  = 27
	  = 81
	> tau/2-pi
	  = 0
	> sin(pi/4)^2
	  = 0.5
	> log10 1000
	  = 3
	> 2.5!
	  = 3.32335
	> 1/(37%3)
	  = 1
	> 1/0
	  = inf
	> log(0)
	  = -inf
	>
	> .2+2.
	  = 2.2
	> --1  -   3 ;  ; ;;;
	  = -2
	> +
	  # No operand
	> aaa
	  # Unknown token
	> ..2
	  # Wrong format of number
	> q


Implement your own language
---

Write a new parser class

	using namespace OPParser;
	class SomeParser: public Parser;

Write tokens

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

Write lexers

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

Add lexers to the parser

	void SomeParser::init() {
	    Parser::init();

	    PLexer lexer(new SomeLexer());
	    lexers[stateNum].push_back(lexer);

	    // More...
	}

Use the parser

	SomeParser parser;
	parser.init();
	parser.parse(?);

	vector <PToken> result;
	parser.finish(result);

	// Do something

