#include "opcalcrepl.hpp"
#include "opcalcnear.hpp"

namespace OPParser {
    // Semicolon
    class GoOnLexer: public Lexer {
    public:
        bool tryGetToken(InputIter &now, const InputIter &end, Parser &parser) {
            if (*now == ';') {
                ((CalcRepl &) parser).write();

                // If bad finish, throw the error with nothing done
                ++now;

                return 1;
            } else {
                return 0;
            }
        }
    };

    void CalcRepl::addLastLexers() {
        {
            PLexer lexer(new GoOnLexer());
            // As number state is unusual
            lexers[stateNum].push_back(lexer);
            lexers[stateOper].push_back(lexer);
        }

        Calc::addLastLexers();
    }

    void CalcRepl::read() {
        // Ask
        (*out).clear();
        (*out)<<"> ";
        (*out).flush();

        // Read
        (*in).clear();
        Input input = L"";
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
            CalcNearData nearresult = fnear(result);

            // Use near value

            // Find x ~= result
            const auto found1 = NearValue.find(nearresult);
            if (found1 != NearValue.end()) {
                (*out)<<"  ~ "<<found1->second<<endl;
            } else {
                // Find x ~= -result
                const auto found2 = NearValue.find(-nearresult);
                if (found2 != NearValue.end()) {
                    (*out)<<"  ~ "<<"- ("<<found2->second<<")"<<endl;
                } else {
                    // Find any ~= result
                    if (nearresult != (CalcNearData)result) {
                        (*out)<<"  ~ "<<nearresult<<endl;
                    }
                }
            }

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
            } catch (const runtime_error &e) {
                (*out)<<"  # "<<e.what()<<endl;
                init();
            }
        }
    }
}
