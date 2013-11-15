calc: opparser.o opcalc.o opcalcnear.o opcalcrepl.o project.o
	clang++ opparser.o opcalc.o opcalcnear.o opcalcrepl.o project.o -o calc
opparser.o: opparser.hpp opparser.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opparser.cpp
opcalc.o: opparser.hpp opcalc.hpp opcalc.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalc.cpp
opcalcnear.o: opparser.hpp opcalc.hpp opcalcnear.hpp opcalcnear.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalcnear.cpp
opcalcrepl.o: opparser.hpp opcalc.hpp opcalcnear.hpp opcalcrepl.hpp opcalcrepl.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 opcalcrepl.cpp
project.o: opparser.hpp opcalc.hpp opcalcnear.hpp opcalcrepl.hpp project.cpp
	clang++ -g -c -w -Wall -Werror -std=c++11 project.cpp
