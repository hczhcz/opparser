cppcheck --enable=all -f --inconclusive -q -v ./
make
valgrind -q ./calc
read var
