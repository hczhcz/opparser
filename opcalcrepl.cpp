#include "opcalcrepl.hpp"

namespace OPParser {
    // Semicolon
    class GoOnLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == ';') {
                ++now;

                ((CalcRepl &) parser).write();
                return 1;
            } else {
                return 0;
            }
        }
    };

    void CalcRepl::init() {
        Calc::init();

        {
            PLexer lexer(new GoOnLexer());
            // As number state is unusual
            lexers[stateNum].push_back(lexer);
            lexers[stateOper].push_back(lexer);
        }
    }

    void CalcRepl::read() {
        // Ask
        (*out).clear();
        (*out)<<"> ";
        (*out).flush();

        // Read
        (*in).clear();
        Input input = "";
        getline((*in), input);
        (*in).sync();

        if (input == exitSign) {
            running = 0;
        } else {
            // Do parsing
            parse(input);
        }
    }

    void CalcRepl::write() {
        if (outStack.empty() && midStack.empty()) {
            // Nothing
        } else {
            CalcData result = finishByData();
            (*out)<<"  = "<<result<<endl;
            (*out).flush();
        }
    }

    void CalcRepl::run(Input exitSign) {
        init();

        running = 1;
        while (running) {
            try {
                read();
                write();
            } catch (runtime_error e) {
                (*out)<<"  # "<<e.what()<<endl;
                init();
            }
        }
    }
}
