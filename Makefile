calc:       opparser.o opcalcrule.o opcalc.o opcalcnear.o opcalcrepl.o project.o
	clang++ opparser.o opcalcrule.o opcalc.o opcalcnear.o opcalcrepl.o project.o -o calc

opparser.o:   opparser.hpp   opparser.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opparser.cpp

opcalcrule.o: opcalcrule.hpp opcalcrule.cpp                  opparser.hpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalcrule.cpp

opcalc.o:     opcalc.hpp     opcalc.cpp                      opparser.hpp opcalcrule.hpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalc.cpp

opcalcnear.o: opcalcnear.hpp opcalcnear.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalcnear.cpp

opcalcrepl.o: opcalcrepl.hpp opcalcrepl.cpp                  opparser.hpp opcalcrule.hpp opcalc.hpp opcalcnear.hpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalcrepl.cpp

project.o:    project.cpp                                    opparser.hpp opcalcrule.hpp opcalc.hpp opcalcnear.hpp opcalcrepl.hpp
	clang++ -g -c -w -Wall -Werror -std=c++11 project.cpp
